from __future__ import annotations

import signal
import time
from typing import Callable, TypeVar


T = TypeVar("T", bound=type)


def wrap_periodic_app(
    bound_class: type,
    *,
    configure_environment: Callable[[], None] | None = None,
):
    class PeriodicApp(bound_class):
        def __init__(self, *args, **kwargs):
            if configure_environment is not None:
                configure_environment()
            super().__init__(*args, **kwargs)

    PeriodicApp.__name__ = "PeriodicApp"
    PeriodicApp.__qualname__ = "PeriodicApp"
    return PeriodicApp


wrap_periodic_app_plugin = wrap_periodic_app
