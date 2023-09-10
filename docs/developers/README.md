This documentation only talks baout the internal code of CORE. That is, anything under the namespace CORE::Internal.

# Project development.

## clang-format

it is crucial that your clang-format version is version 15 so that it is the same that is used in github actions. To download this version and set it as default you can use these commands in ubuntu:

```
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
echo "deb http://apt.llvm.org/focal/ llvm-toolchain-focal main" | sudo tee -a /etc/apt/sources.list
sudo apt-get update
sudo apt-get install -y clang-format-15
sudo update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-15 1000
```

And after that, `clang-format --version` should be 15. Use the script: ./scripts/clang_format_all_files.sh before merging with main to pass that individual github action.

Note that if you wish to not use clang-format somewhere in your code, you just need to add these comments:

```
// clang-format off

// ... Code that is not formatted...

// clang-format on
```
