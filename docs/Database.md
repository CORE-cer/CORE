## Arcvhivos creados
# core_server/../database.hpp
- Clase Database para construir el modelo de la base de datos
- El constructor llama a 
  - connect_database() -> Usa pqxx para conectar C++ a la base de datos de postgres deseada
  - create_streams_names_table() -> Crea la tabla que relaciona el nombre se un stream y su id unico
  - create_event_names_table() -> Crea la tabla que relaciona el nombre de un evento con su id de tipo

- add_event_type() -> Crea una tabla por cada tipo de evento
- add_event() -> Falto por implementar, inserta un evento a la tabla
- get_event_table_name() -> Utilizada por add_event() para encontrar el nombre del evento con su id de tipo
- add_stream_type() -> Crea una tabla por cada stream 
- create_streams_table() -> Crea la tabla para relacionar los eventos con sus streams provenientes
- execute_query() -> Ejecuta todas las consultas anteriores con pqxx

# unit_tests/../database.hpp
Creacion del Test Case para conectar a base de datos y crear mnodelo
- create_database() -> Usa los parametros dados para crear una base de datos (*El return del catch es para conectar con la base de datos aun cuando ya esta creada, para facilitar ejecucion*)
- Se crean eventos arbitrarios y se guardan los nombres en un vector
- Se llama a add_event_type() para crear la tabla del tipo de evento
- Se crea un stream arbitrario y se llama a add_stream_type()
- Finalmente se llama a create_streams_table