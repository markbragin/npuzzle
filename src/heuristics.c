#include "heuristics.h"

#include <bits/stdint-uintn.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

static int linear_conflicts_in_rows(const Board board, unsigned size);
static int linear_conflicts_in_cols(const Board board, unsigned size);

unsigned manhattan_dist(const Board board, unsigned size)
{
  int ans, i, j, i1, j1, val;

  ans = 0;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      val = board[i * size + j];
      if (val == 0)
        continue;
      i1 = (val - 1) / size;
      j1 = (val - 1) % size;
      ans += abs(i1 - i) + abs(j1 - j);
    }
  }
  return ans;
}

unsigned hamming_dist(const Board board, unsigned size)
{
  int i, ans;

  for (i = 0, ans = 0; i < size * size - 1; i++)
      if (board[i] != i + 1)
          ans++;
  return ans;
}

unsigned linear_conflicts(const Board board, unsigned size)
{
    return linear_conflicts_in_rows(board, size) +
           linear_conflicts_in_cols(board, size) +
           manhattan_dist(board, size);
}

static int linear_conflicts_in_rows(const Board board, unsigned size) {
    int i, j, k, to_move, nconflicts, max, maxIdx;
    int f[16];
    Tile tj, tk;

    to_move = 0;
    for (i = 0; i < size; i++) {
        memset(f, 0, sizeof(int) * 16);
        nconflicts = 0;
        for (j = 0; j < size; j++) {
            tj = board[i * size + j];
            if (tj == 0)
                continue;
            if ((tj - 1) / size == i) {
                for (k = j + 1; k < size; k++) {
                    tk = board[i * size + k];
                    if (tk == 0)
                        continue;
                    if ((tk - 1) / size == i && tj > tk) {
                        f[j]++;
                        f[k]++;
                        nconflicts += 2;
                    }
                }
            }
        }
        while (nconflicts > 0) {
            max = 0;
            maxIdx = 0;
            for (j = 0; j < size; j++) {
                if (f[j] > max) {
                    max = f[j];
                    maxIdx = j;
                }
            }
            tk = board[i * size + maxIdx];
            for (j = 0; j < size; j++) {
                if (j == maxIdx)
                    continue;
                tj = board[i * size + j];
                if ((tj != 0 && (tj > tk && j < maxIdx)) ||
                    (tj < tk && j > maxIdx)) {
                    f[j]--;
                    nconflicts -= 2;
                }
            }
            to_move += 2;
        }
    }
    return to_move;
}

static int linear_conflicts_in_cols(const Board board, unsigned size) {
    int i, j, k, to_move, nconflicts, max, maxIdx;
    int f[16];
    Tile tj, tk;

    to_move = 0;
    for (j = 0; j < size; j++) {
        memset(f, 0, sizeof(int) * 16);
        nconflicts = 0;
        for (i = 0; i < size; i++) {
            tj = board[i * size + j];
            if (tj == 0)
                continue;
            if ((tj - 1) % size == j) {
                for (k = i + 1; k < size; k++) {
                    tk = board[k * size + j];
                    if (tk == 0)
                        continue;
                    if ((tk - 1) % size == j && tj > tk) {
                        f[i]++;
                        f[k]++;
                        nconflicts += 2;
                    }
                }
            }
        }
        while (nconflicts > 0) {
            max = 0;
            maxIdx = 0;
            for (i = 0; i < size; i++) {
                if (f[i] > max) {
                    max = f[i];
                    maxIdx = i;
                }
            }
            tk = board[maxIdx * size + j];
            for (i = 0; i < size; i++) {
                if (i == maxIdx)
                    continue;
                tj = board[i * size + j];
                if ((tj != 0 && (tj > tk && i < maxIdx)) ||
                    (tj < tk && i > maxIdx)) {
                    f[i]--;
                    nconflicts -= 2;
                }
            }
            to_move += 2;
        }
    }
    return to_move;
}
