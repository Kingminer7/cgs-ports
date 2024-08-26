1. in CMakeLists.txt, replace 
```cmake
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
```
with
```cmake
if ("${GEODE_TARGET_PLATFORM}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
    add_definitions(-DGLES_SILENCE_DEPRECATION)
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
```
2. add iOS to mod.json
3. use special build action (see below, don't forget to set GH_PAT in actions secrets)
4. hope it works, either you get lucky, or you choose between removing crap or finding bindings

have fun btw

---------------------------
github action
```yml
name: Build (mod name)

on:
  push:
    branches:
      - (mod branch)

jobs:
  build:
    continue-on-error: true
    strategy:
      fail-fast: false # nevermind
      matrix:
        config:
        - name: Windows
          os: windows-latest

        - name: macOS
          os: macos-latest

        - name: Android32
          os: ubuntu-latest
          target: Android32

        - name: Android64
          os: ubuntu-latest
          target: Android64

    name: Build (${{ matrix.config.name }})
    runs-on: ${{ matrix.config.os }}

    steps:
      - name: Checkout
        uses: actions/checkout@v4
        with:
          fetch-depth: 0
      
      - name: Catch up
        # see https://github.com/actions/checkout/issues/439
        run: git pull origin main --no-rebase

      - name: Build the mod
        uses: geode-sdk/build-geode-mod@main
        with:
          bindings: geode-sdk/bindings
          bindings-ref: main
          combine: true
          target: ${{ matrix.config.target }}
          
  build-ios:
    name: Build for iOS
    runs-on: macos-latest
    continue-on-error: true
    steps:
      - name: Checkout
        uses: actions/checkout@v4
        with:
          fetch-depth: 0
      
      - name: Catch up
        # see https://github.com/actions/checkout/issues/439
        run: git pull origin main --no-rebase

      - name: Build SPECIFICALLY for iOS which is just ridiculous
        uses: geode-catgirls/build-geode-mod@main
        with:
          bindings: geode-catgirls/bindings
          target: iOS
          gh-pat: ${{ secrets.GH_PAT }}
          # super secret
          
  package:
    name: Package and release mod
    runs-on: ubuntu-latest
    needs: ['build', 'build-ios']
    steps:
      - uses: geode-sdk/build-geode-mod/combine@main
        id: build
        name: Combine mods

      - uses: actions/upload-artifact@v4
        name: Upload artifact
        with:
          name: Build Output
          path: ${{ steps.build.outputs.build-output }}
```