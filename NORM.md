# webserv norm :

## File Structure

- Header Files (.hpp/.h):
  - Header folder must have a default.hpp which will be used for including in .cpp
  - Include declarations of classes, functions, constants, and macros.
  - Use include guards or #pragma once to prevent multiple inclusions.
- Source Files (.cpp):
  - Contain the implementation of functions and classes.

## Naming Conventions
- Variables: snake_case (e.g., int student_age;)
- Functions: camelCase (e.g., void calculateSum();)
- Classes: PascalCase (e.g., class UserAccount {};)
- Constants: ALL_CAPS_SNAKE_CASE (e.g., const int MAX_SIZE = 100;)
- Namespaces: lowercase (e.g., namespace utils {})

## Indentation and Spacing
- Use tabs with a size equivalent to 4 spaces for indentation.
- Place one space before and after binary operators (e.g., a + b).
- Use a single blank line between function definitions and logical blocks.

## Comments
Use // for single-line comments and /* ... */ for multi-line comments. \
Always explain why a decision was made, not what the code does.
- Example:
```cpp
// Adjusting balance to account for monthly interest
balance += interest;
```

## Function Design
Keep functions small and focused on a single task. \
Use descriptive names for function parameters.
- Example:
```cpp
double calculateArea(double radius) {
    return 3.14159 * radius * radius;
}
```
## Error Handling
Use exceptions for error handling, and always catch exceptions where needed.
- Example:
```cpp
try {
    openFile("data.txt");
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```
## Smart Pointers
Prefer smart pointers (std::unique_ptr, std::shared_ptr) over raw pointers for better memory management.
- Example:
```cpp
std::unique_ptr<int> ptr = std::make_unique<int>(10);
```
## Standard Library
Use the STL (e.g., std::vector, std::map) instead of manual memory management or arrays when appropriate.

## Include Statements
Use angled brackets (< >) for standard libraries and quotes (" ") for project-specific files. \
Group and order includes logically:
- Standard headers
- Third-party libraries
- Project-specific headers

## Code Formatting
Use a consistent style for braces:
```cpp
void example() {
    if (true) {
	    // Code block
    }
}
```
## Using _t Suffix for typedef
The _t suffix is a convention to indicate a type definition or alias. It helps distinguish custom types from standard or built-in types. \
Use _t consistently for all user-defined types created with typedef or using. \
Guidelines:
- Descriptive Names:
  - The base name should clearly describe the type's purpose.
  - Add _t to the end of the name for clarity.
  - Example:
	```cpp
	typedef unsigned int uint_t; // Alias for unsigned int
	```
- Consistency Across Codebase:
  - Always use _t for typedefs to maintain consistency and readability.
  - Example:
	```cpp
	typedef struct {
 	  int id;
	  char name[50];
	} student_t;
	```
Examples:
- Basic Alias:
```cpp
typedef double distance_t;
distance_t radius = 5.5;
```
- Pointer Alias:
```cpp
typedef char* string_t;
string_t greeting = "Hello, World!";
```
- Struct Alias:
```cpp
typedef struct {
    int x;
    int y;
} point_t;

point_t p1 = {10, 20};
```
