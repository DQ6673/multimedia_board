set(srcs "gt911.c")

set(includes ".")

set(requires 
                "driver"
                "iic"
)

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       REQUIRES ${requires}
                       )