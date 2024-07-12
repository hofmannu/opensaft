# Contributing



## Coding conventions

- no `return` statement at the end of void functions
- declaration of `public` members comes before `private` members
- avoid `enum` in favor of `enum class`
- all includes of external libraries are done through `<>` brackets and the ones defined within this repository should be added using `""`
- all repository include paths should be defined relative to `src` directory

## Class structure

- `public` members first