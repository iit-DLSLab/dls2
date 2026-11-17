from __future__ import annotations

from dataclasses import dataclass
from datetime import datetime
from threading import Condition, Lock
from typing import Any, Callable

from .reader import Reader
from .writer import Writer


@dataclass
class InputInfo:
    latest_period_ms: float = 0.0
    latest_timestamp: datetime | None = None
    missed_sequence_ids: int = 0
    are_inputs_required_on_activation: bool = False
    topic_name: str = ""
    reader: Any = None


@dataclass
class OutputInfo:
    writer: Any = None
    topic_name: str = ""


class Plugin:
    def __init__(self, plugin_id: str, domain: int | str) -> None:
        self.plugin_id = plugin_id
        self.domain = domain

        self.unique_outputs_cv = Condition()
        self.unique_outputs_mutex = Lock()

        self.missing_inputs: list[str] = []
        self.common_outputs: list[str] = []

        self.input_info_mutex = Lock()
        self.output_info_mutex = Lock()

        self.input_info: list[InputInfo] = []
        self.output_info: list[OutputInfo] = []
        self.inputs_map: dict[str, int] = {}
        self.outputs_map: dict[str, int] = {}

        self._rpc_srvc_map: dict[str, Any] = {}

    def build_input(
        self,
        topic_name: str,
        topic_data_type: Any,
        data: Any,
        auxiliary_callback: Callable[[], None] | None = None,
        required_on_activation: bool = True,
    ) -> Any:
        callback = auxiliary_callback or (lambda: None)
        reader = Reader(self.domain, topic_data_type, data, topic_name)

        wrapped_reader = _ReaderWithCallback(reader, callback)

        info = InputInfo(
            latest_timestamp=datetime.now(),
            are_inputs_required_on_activation=required_on_activation,
            topic_name=topic_name,
            reader=wrapped_reader,
        )

        with self.input_info_mutex:
            self.input_info.append(info)
            self.inputs_map[topic_name] = len(self.input_info) - 1

        return wrapped_reader

    def build_output(
        self,
        topic_name: str,
        topic_data_type: Any,
        data: Any,
    ) -> Any:
        writer = Writer(self.domain, topic_data_type, data, topic_name)
        info = OutputInfo(writer=writer, topic_name=topic_name)

        with self.output_info_mutex:
            self.output_info.append(info)
            self.outputs_map[topic_name] = len(self.output_info) - 1

        return writer

    def read(self, name: str | None = None) -> None:
        if name is None:
            with self.input_info_mutex:
                for info in self.input_info:
                    self._read_input(info)
            return

        try:
            input_idx = self.inputs_map[name]
        except KeyError as exc:
            raise RuntimeError(f"Input name '{name}' not found in the plugin inputs.") from exc

        with self.input_info_mutex:
            self._read_input(self.input_info[input_idx])

    def write(self, name: str | None = None) -> None:
        if name is None:
            with self.output_info_mutex:
                for info in self.output_info:
                    self._publish_output(info.writer)
            return

        try:
            output_idx = self.outputs_map[name]
        except KeyError as exc:
            raise RuntimeError(f"Output name '{name}' not found in the plugin outputs.") from exc

        with self.output_info_mutex:
            self._publish_output(self.output_info[output_idx].writer)

    def are_inputs_receiving_data(self) -> bool:
        are_inputs_receiving_data = True
        self.missing_inputs.clear()

        with self.input_info_mutex:
            for info in self.input_info:
                if not info.are_inputs_required_on_activation:
                    continue
                if not _reader_is_receiving_data(info.reader):
                    self.missing_inputs.append(info.topic_name)
                    are_inputs_receiving_data = False

        return are_inputs_receiving_data

    def are_outputs_unique(self) -> bool:
        # The lightweight Python reader/writer helpers do not currently expose
        # enough discovery metadata to implement the same uniqueness check as
        # the C++ plugin base. Keep the hook so higher-level code can share the
        # same structure and override this behavior when richer DDS bindings are
        # available.
        self.common_outputs.clear()
        return True

    def create_replier(self, service_name: str, replier: Any) -> None:
        self._rpc_srvc_map[service_name] = replier

    def create_requester(self, service_name: str, requester: Any) -> None:
        self._rpc_srvc_map[service_name] = requester

    def send_request(self, service_name: str, data: Any) -> None:
        self._rpc_srvc_map[service_name].sendRequest(data)

    def send_reply(self, service_name: str, data: Any) -> None:
        self._rpc_srvc_map[service_name].sendReply(data)

    def get_data(self, service_name: str) -> Any:
        service = self._rpc_srvc_map[service_name]
        if hasattr(service, "getData"):
            return service.getData()
        return None

    def delete_services(self) -> None:
        self._rpc_srvc_map.clear()

    def _read_input(self, info: InputInfo) -> None:
        if hasattr(info.reader, "read"):
            info.reader.read()
        self._update_input_info(info)

    def _update_input_info(self, info: InputInfo) -> None:
        info.latest_timestamp = datetime.now()
        if hasattr(info.reader, "get_latest_period_ms"):
            info.latest_period_ms = info.reader.get_latest_period_ms()
        if hasattr(info.reader, "getMissedSequenceIds"):
            info.missed_sequence_ids = info.reader.getMissedSequenceIds()

    @staticmethod
    def _publish_output(writer: Any) -> None:
        if hasattr(writer, "publish"):
            writer.publish()
        elif hasattr(writer, "write"):
            writer.write()
        else:
            raise RuntimeError("Writer object must expose either publish() or write().")


class _ReaderWithCallback:
    def __init__(self, reader: Any, callback: Callable[[], None]) -> None:
        self._reader = reader
        self._callback = callback

    def __getattr__(self, name: str) -> Any:
        return getattr(self._reader, name)

    def read(self) -> Any:
        data = self._reader.read() if hasattr(self._reader, "read") else None
        self._callback()
        return data


def _reader_is_receiving_data(reader: Any) -> bool:
    if hasattr(reader, "is_receiving_data"):
        return bool(reader.is_receiving_data())
    if hasattr(reader, "hasStartedReceivingData"):
        return bool(reader.hasStartedReceivingData())
    if hasattr(reader, "getData"):
        data = reader.getData()
        return data is not None
    return True
