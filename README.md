# Webserv

A simple web server written in C++.

## Development

### Diagrams

#### Class diagram

![Class diagram](./docs/class_diagram.png)

#### Sequence diagram

![Sequence diagram](./docs/sequence_diagram.png)

### Macros

- `DEBUG`: Set the debug level. The higher the level, the higher the severity of the log messages. The default level is
  3, for only error messages. Minimum level is 0, for all debug messages.
    - `0`: Debug messages
    - `1`: Info messages
    - `2`: Warning messages
    - `3`: Error messages

- `LOG_ERROR`: Log error messages.
- `LOG_WARNING`: Log warning messages.
- `LOG_INFO`: Log info messages.
- `LOG_DEBUG`: Log debug messages.

### Build

```bash
make
```

### Run

```bash
./webserv [config_file]
```

