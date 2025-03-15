# helloasio

## Requirements

  - [CMake](https://cmake.org/)

## Development

Setup with build-system with `ninja`.

```sh
cmake -S . -Bbuild -GNinja -DCMAKE_BUILD_TYPE=Debug
```

Compile using `ninja`.


```sh
ninja -C build
```

### Link compile_commands.json

**Unix**:

```sh
ln -sfn ./build/compile_commands.json .
```

**Windows**:

```ps1
New-Item -ItemType SymbolicLink -Path "compile_commands.json" -Target "./build/compile_commands.json"
```

### Tips working with git

Clone repo as bare and then create a `worktree` with it.

```sh
git clone --bare git@github.com:mnerv/helloasio.git
```

`cd` into the `helloasio.git` directory and then use the command below to create
a work tree.

```sh
git worktree add {path} {branch}
```

`{path}`: The path you want to put the working tree.

`{branch}`: The branch you want to work with.


Example:

```sh
git worktree add trunk trunk
```

To remove `worktree` you can use the following command

```sh
git worktree remove {path}
```

Or you can also just remove the directory

```sh
rm -rf {path}
```

And then you'll need to use the command below to remove stale `worktrees`.

```sh
git worktree prune
```
