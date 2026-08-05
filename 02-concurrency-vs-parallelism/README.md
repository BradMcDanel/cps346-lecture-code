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

## Part 1: estimate_pi

This program estimates pi by adding up 500 million thin slices under a curve.

- Run `./estimate_pi 1` and write down the time. The 1 here means we run on one thread.
- Now run it again with 2, 4, 8, 16, and 32 threads. Write down each time.
- For each run, compute the speedup: `baseline time / this time`.
- At what number of threads does the speedup stop getting better?

## Part 2: fetch_pages

This program simulates downloading 64 web pages (using sleep).

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
  Which one is using your machine better? Why?
- Finally, try `./fetch_pages 128`. Explain the result.

