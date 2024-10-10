# Args Library

The `Args` library is a simple C++ command-line argument parser designed to handle various types of arguments like booleans, strings, integers, floats, doubles, and arrays of these types. It allows you to define a schema for parsing command-line arguments and retrieve their values.

## Features

- Supports multiple argument types:
  - Boolean
  - String
  - Integer
  - Float
  - Double
  - Arrays of strings, integers, and floats
- Allows specifying mandatory arguments
- Easy-to-use API for retrieving parsed values

## Installation

To use the `Args` library in your project, include the necessary headers and link the implementation files.

```cpp
#include "args.h"
```

## Usage

### Define a Schema

You define a schema as a format string where each key is represented by a single letter, followed by an optional character indicating the type of the argument. The schema string tells the parser how to interpret each command-line argument.

### Format String Syntax

- `x` : Boolean (no type specifier)
- `x*` : String
- `x#` : Integer
- `x#%` : Float
- `x##` : Double
- `x[*]` : Vector of strings
- `x[#]` : Vector of integers
- `x[#%]` : Vector of floats
- `!` : Mark an argument as required

### Example

```cpp
#include "args.h"

int main(int argc, char** argv) {
    // Define the schema: b - boolean, s* - string, i# - integer, f#% - float
    Args args("b s* i# f#%", argc, argv);
    
    // Get parsed arguments
    bool flag = args.getBoolean('b');
    std::string name = args.getString('s');
    int count = args.getInt('i');
    float value = args.getFloat('f');
    
    return 0;
}
```

### Command-line Example

For a schema `"b s* i# f#%"`, the following command line:

```
./app -b true -s Hello -i 42 -f 3.14
```

will parse:
- `b`: `true` (Boolean)
- `s`: `"Hello"` (String)
- `i`: `42` (Integer)
- `f`: `3.14` (Float)

### Available Methods

- `bool getBoolean(char arg)` — Get a boolean value.
- `std::string getString(char arg)` — Get a string value.
- `int getInt(char arg)` — Get an integer value.
- `float getFloat(char arg)` — Get a float value.
- `double getDouble(char arg)` — Get a double value.
- `std::vector<std::string> getStringArray(char arg)` — Get an array of strings.
- `std::vector<int> getIntArray(char arg)` — Get an array of integers.
- `std::vector<float> getFloatArray(char arg)` — Get an array of floats.

## Error Handling

The library will automatically validate the schema and arguments provided. If any required argument is missing or invalid, an error will be raised.
