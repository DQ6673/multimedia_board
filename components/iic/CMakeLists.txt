set(srcs "iic.c")

set(includes ".")

set(requires "driver"
                )


idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       REQUIRES ${requires}
                       )