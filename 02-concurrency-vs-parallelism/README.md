## Build instructions

```
cd 02-concurrency-vs-parallelism/
mkdir build
cd build
cmake ..
make

# now, all executables have been built.
# if you modify any cpp file, you need to re-run make (`cmake ..` does not need to be re-run, just `make`)
```

## Before you start

Both programs take a thread count as their first argument, and both print how
many cores your machine reports. Write that number down.

If a run seems way too slow, you can pass a smaller problem size as a second
argument (e.g. `./estimate_pi 1 100000000`). Just use the same size for every
run or your times will not be comparable.


## Part 1: estimate_pi

Estimates pi by adding up 500 million thin slices under a curve.

- Run `./estimate_pi 1` and write down the time. This is your baseline.
- Now run it again with 2, 4, 8, 16, and 32 threads. Write down each time.
- For each run, compute the speedup: `baseline time / this time`.
- At what number of threads does the speedup stop getting better?
  * Stop here - we will talk about this together.


## Part 2: fetch_pages

Downloads 64 web pages.

- Run `./fetch_pages 1` and write down the time.
- Now run it with 2, 4, 8, 16, 32, and 64 threads.
- Compute the speedup for each one.
- Where does this one stop getting better?


## Part 3: put them side by side

Make a table like this (can fill in below, write down on paper, or other tool):

```
threads    estimate_pi time   speedup      fetch_pages time   speedup
-------    ----------------   -------      ----------------   -------
   1
   2
   4
   8
  16
  32
  64
```

- The two columns stop improving in different places. Go look at the source for
  both programs. What is each thread actually doing while it runs, and how
  would that explain the difference?
- Efficiency is `speedup / threads`. Compute it for both programs at 8 threads.
  Which one is using your machine better, and is that the same as which one got
  faster?
- Try `./fetch_pages 128`. Explain the result.
