# Developer hints  {#developer_hints}
## Running node without console layer
```bash
/usr/bin/dls2/child_process_launcher <ID> <lib_name> <lib_type> <ID/robotName>
```
Usually the ID and lib_name are equal. Robot Name is used istead of ID for lib types: `motion_generator`, `controller`,`estimator`.