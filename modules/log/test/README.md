# How to launch
0. After compilation and installation of dls2, go in the folder _dls2/bin/Release_
1. launch server
    ```
    fastdds discovery -p 11818 -i 7 -l 127.0.0.1
    ```
2. launch publishers (test_notify_events spawner <num_pubs> <test_time_sec>)
    E.g.
    ```
    ./test_notify_events spawner 30 60
    ```
3. launch subscriber (./test_notify_events listener <num_pubs> <test_time_sec> )
    E.g.
    ```
    ./test_notify_events listener 30 60
    ```

    Ensure to have the same number of publishers and the same test time for step 2 and 3.

4. launch performance checker
    ```
    ./test_notify_events checker
    ```