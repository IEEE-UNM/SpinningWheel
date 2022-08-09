# SpinningWheel
A qt5 spinning wheel widget used to randomized choices.
This is mainly used a submodule library used by [import-random](https://github.com/IEEE-UNM/import-random).

# Usage
Clone this repository and add the cloned repository as a subdirtory in CMakeLists.txt
```sh
git clone https://github.com/IEEE-UNM/SpinningWheel.git
```

CMakeLists.txt in the project file
``` cmake
...
add_subdirectory(path/to/SpinningWheel)
```

# TODO
  * [ ] Fixed bug with pie drawing when there are more items
  * [ ] Added clicking to spin support for the wheel
