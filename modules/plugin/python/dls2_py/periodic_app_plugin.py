from __future__ import annotations

from typing import Any, Callable

from .env_bootstrap import setup_environment
from .periodic_app import wrap_periodic_app
from .plugin import Plugin


def wrap_periodic_app_plugin(
    bound_class: type,
    *,
    configure_environment: Callable[[], None] | None = None,
):
    periodic_app_base = wrap_periodic_app(
        bound_class,
        configure_environment=configure_environment,
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
):
    return setup_environment(app_id, module_file=module_file)
