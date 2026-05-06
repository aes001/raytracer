# RayTracer

A C++ ray tracer project by Racc (`aes001`) focused on rendering OBJ triangle meshes and experimenting with acceleration structures. The project builds multiple ray tracer binaries that run as normal ray tracer or a benchmark only version that export timing and traversal statistics.

## Features

- OBJ mesh loading through `rapidobj`
- Triangle primitive support with mesh transforms
- PNG output through `stb_image_write`
- PPM image buffer utilities in `raytracer/RaccPPM`
- RTIOW based camera, rays, hittables, vectors, and scene utilities
- Multiple acceleration-structure targets:
  - baseline `RayTracer`
  - binary top-down median split BVH
  - BVH4 top-down even split
  - BVH8 top-down even split
  - BVH2 top-down naive SAH
  - BVH2 bottom-up SAH
- Benchmark executables that write CSV timing/counting data

## Requirements

- CMake 3.30 or newer
- A C++ compiler supported by CMake
- Git/network access during first configure, because CMake fetches `fmt` with `FetchContent`

Vendored dependencies live under `libs/`:

- `rapidobj`
- `stb-image`

## Build

Configure and build from the repository root:

```bash
cmake -B build -S .
cmake --build build
```

For a debug build:

```bash
cmake -B build/debug -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build/debug
```

The top-level CMake file prevents in-source builds, so always use a separate build directory.

## Executables

The project generates normal render targets and matching benchmark targets prefixed with `BM_`.

| Target | Description |
| --- | --- |
| `RayTracer` | Baseline renderer without BVH acceleration |
| `RayTracer-TD_ES_BVH2` | Binary BVH, top-down median/even split |
| `RayTracer-TD_ES_BVH4` | BVH4, top-down even split |
| `RayTracer-TD_ES_BVH8` | BVH8, top-down even split |
| `RayTracer-TD_NSAH_BVH2` | BVH2, top-down naive SAH |
| `RayTracer-BU_SAH_BVH2` | BVH2, bottom-up SAH |
| `BM_<target>` | Benchmark build for the corresponding target |

Example:

```bash
cmake --build build --target RayTracer-TD_ES_BVH2
cmake --build build --target BM_RayTracer-TD_ES_BVH2
```

## Usage

Run a renderer from the build output directory or pass paths relative to your current working directory.

```bash
./build/raytracer/RayTracer path/to/model.obj
```

For non-benchmark builds, an optional second argument sets the sample count:

```bash
./build/raytracer/RayTracer-TD_ES_BVH2 path/to/model.obj 100
```

If no OBJ path is provided, `main.cpp` attempts to load:

```text
../../data/suzanne.obj
```

That file is not included in this repository tree, so passing an explicit OBJ path is the only way to run the program.

## Output

Normal renderer builds write:

```text
OutputPNG.png
```

Benchmark builds render at a smaller resolution and write CSV files such as:

```text
Timings-<scene>_<tree>_<approach>_<split>.csv
Countings-<scene>_<tree>_<approach>_<split>.csv
Histogram-<scene>_<tree>_<approach>_<split>.csv
```

Counting and histogram files are produced for BVH-enabled benchmark targets. The baseline benchmark writes timing data only.

## Benchmarking

Build one of the `BM_` targets:

```bash
cmake --build build --target BM_RayTracer-TD_ES_BVH8
```

Run it with an OBJ mesh:

```bash
./build/raytracer/BM_RayTracer-TD_ES_BVH8 path/to/model.obj
```

Benchmark builds perform warm-up renders, then collect render timings for several sample counts. BVH benchmark builds also collect traversal/counting statistics.


## Notes

- The renderer currently expects OBJ input that can be parsed and triangulated by `rapidobj`.
- Output files are written to the process working directory.
