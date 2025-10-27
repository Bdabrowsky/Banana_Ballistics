set(CMakeDemo_SRC
    config.h
    grain.h
    io.h
    motor.h
    nozzle.h
    propellant.h
    tank.h
)

# Form the full path to the source files...
PREPEND(CMakeDemo_SRC)
# ... and pass the variable to the parent scope.
set(CMakeDemo_SRC ${CMakeDemo_SRC}  PARENT_SCOPE)