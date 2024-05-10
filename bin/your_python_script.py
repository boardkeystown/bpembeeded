import my_cpp_module  # Import the module you created in C++

def passobject(fooBar : my_cpp_module.FooBar):
    print(f'from cpp {fooBar.a=} {fooBar.f=} {fooBar.str=}')
    fooBar.a = 555
    fooBar.f = 123.123
    fooBar.str = "changed in python"

def entrypoint():
    my_cpp_module.counter()  # Call the C++ function
# Run the entrypoint function if this script is executed
if __name__ == "__main__":
    print("main gets called once!")
    entrypoint()