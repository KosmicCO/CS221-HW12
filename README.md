# CS221-HW12

### Compiler Instructions
If the make file is used, everything should compile just fine.

### Run Information
Parameters (num_threads changed):
`tsp challenge.tsv 2000 0.3 [num_threads]`

| Thread Count | Real Time | User Time |
|--------------|-----------|-----------|
| 1            | 2:49.453  | 2.49.303  |
| 2            | 1:25.104  | 2:48.870  |
| 4            | 0:51.456  | 3:23.045  |

The distance of the shortest path that the 4 threaded run produced was 7647.53 (included as shortest.tsv)
