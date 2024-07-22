# Contributing

## Coding conventions

- no `return` statement at the end of void functions
- avoid `enum` in favor of `enum class`
- all includes of external libraries are done through `<>` brackets and the ones defined within this repository should be added using `""`
- all repository include paths should be defined relative to `src` directory, so even if a file is in the same subdirectory, it should be included with `#include "subdirectory/file.h"` instead of `#include "file.h"`

## Class structure

- `public` members first, then `protected`, then `private`: public interface is most relevant and needs to be the first thing visible
- `#pragma once` at the top of each header file


## Namespace

- the entire library should be encapsulated into `opensaft` namespace to make the life of any consumer easier
