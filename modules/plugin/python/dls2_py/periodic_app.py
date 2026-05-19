from __future__ import annotations

import signal
import time
from typing import Callable, TypeVar


T = TypeVar("T", bound=type)


def wrap_periodic_app_plugin(
    bound_class: type,
    *,
    configure_environment: Callable[[], None] | None = None,
    poll_period_s: float = 0.05,
):
    class PeriodicAppPlugin(bound_class):
        def __init__(self, *args, **kwargs):
            if configure_environment is not None:
                configure_environment()
            super().__init__(*args, **kwargs)

        def serve_forever(self) -> None:
            def stop(_signum, _frame):
                self.shutdown()

            signal.signal(signal.SIGINT, stop)
            signal.signal(signal.SIGTERM, stop)
            self.start()

            while True:
                if self.state() == "quit":
                    return
                time.sleep(poll_period_s)

    PeriodicAppPlugin.__name__ = "PeriodicAppPlugin"
    PeriodicAppPlugin.__qualname__ = "PeriodicAppPlugin"
    return PeriodicAppPlugin
