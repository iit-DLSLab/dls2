from __future__ import annotations

from typing import Any, Callable

from dls2_periodic_app_bindings import PeriodicApp as _PeriodicApp

from .env_bootstrap import configure_runtime_configs, setup_environment
from .periodic_app import wrap_periodic_app
from .plugin import Plugin


def wrap_periodic_app_plugin(
    bound_class: type,
    *,
    configure_environment: Callable[[], None] | None = None,
    poll_period_s: float = 0.05,
):
    periodic_app_base = wrap_periodic_app(
        bound_class,
        configure_environment=configure_environment,
        poll_period_s=poll_period_s,
    )

    class PeriodicAppPlugin(periodic_app_base, Plugin):
        def __init__(self, app_id: str, domain: int | str, *args: Any, **kwargs: Any):
            Plugin.__init__(self, plugin_id=app_id, domain=domain)
            super().__init__(*args, **kwargs)

        def check_activation(self) -> bool:
            return self.are_inputs_receiving_data() and self.are_outputs_unique()

    PeriodicAppPlugin.__name__ = "PeriodicAppPlugin"
    PeriodicAppPlugin.__qualname__ = "PeriodicAppPlugin"
    return PeriodicAppPlugin


def create_periodic_app_plugin(
    app_id: str,
    *,
    module_file: str | None = None,
    poll_period_s: float = 0.05,
):
    periodic_app_plugin = setup_environment(app_id, module_file=module_file)
    if poll_period_s == 0.05:
        return periodic_app_plugin

    class _BoundPeriodicApp(_PeriodicApp):
        def __init__(self) -> None:
            super().__init__(app_id)

    def _configure_environment() -> None:
        configure_runtime_configs(app_id, module_file=module_file)

    return wrap_periodic_app_plugin(
        _BoundPeriodicApp,
        configure_environment=_configure_environment,
        poll_period_s=poll_period_s,
    )
