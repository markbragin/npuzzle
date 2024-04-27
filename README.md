# Npuzzle solver

## Usage

```
USAGE:
    npuzzle FILE [options]

    Default options are --greedy --linear

ARGS:
    FILE
        File with input board.

OPTIONS:
    -g, --greedy
        Use greedy best first search algorithm.

    -a, --astar
        Use A* algorithm.

    -i, --idastar
        Use IDA* algorithm.

    -h, --hamming
        Use hamming distance heuristics.

    -m, --manhattan
        Use manhattan distance heuristics.

    -l, --linear
        Use linear conflicts heuristics.

    -5, --database555
        Use pattern database 5-5-5 heuristics.

    -6, --database663
        Use pattern database 6-6-5 heuristics.

        --create555
        Create pattern database 5-5-5.

        --create663
        Create pattern database 6-6-3.

        --help
        Print this help message.
```

## Installation

```
git clone https://github.com/markbragin/npuzzle.git
cd npuzzle
make release
```

then run

`./npuzzle examples/3_31.txt -a -l`

## Examples

![npuzzle](./examples/example.png "npuzzle")

## Algorithms

- **Greedy best first search**. Fast but finds not optimal solution.
- **A***. Slow and memory consuming but finds optimal solution.
- **IDA***. Slower than A* but consumes less memory and finds optimal solution.

## Heuristics

In order of decreasing search time:
- Hamming distance
- Manhattan distance
- Linear conflicts
- Pattern database 5-5-5 (4x4 boards)
- Pattern database 6-6-3 (4x4 boards)

Pattern databases faster than other heuristics, but it take time to generate
them.

On my machine with i3 2nd gen and 8GB RAM:
- PDB 5-5-5 takes about 1 minute to generate and uses 26 MB of storage space.
- PDB 6-6-2 takes about 9 minute to generate and uses 188 MB of storage space.

## Limitations

- Allowed boards not bigger than 16x16. (I don't think one needs more :D).
- Solves only puzzles with the blank tile in the right bottom corner in the
goal state.
- Uses not the best heuristics, so finding optimal path to the hardest
4x4 puzzles takes a lot of time [and memory] :(
