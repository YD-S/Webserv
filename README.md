# Webserv

A simple web server written in C++.

## Development

### Diagrams

#### Class diagram

![Class diagram](./docs/class_diagram.svg)

#### Sequence diagram

![Sequence diagram](./docs/sequence_diagram.svg)

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

### Request builder

The request class can be built using a builder pattern.
Here is an example:

```cpp
HttpRequest req = HttpRequest()
        .setMethod("POST")
        .setPath("/index.html")
        .setVersion("HTTP/1.1")
        .addHeader("Content-Type", MimeTypes::getType("html"))
        .addHeader("Content-Length", "12")
        .setBody("Hello World!");
```

### Response builder

The response class can be built using a builder pattern.
Here is an example:

```cpp
HttpResponse response = HttpResponse()
        .setVersion("HTTP/1.1")
        .setStatus(HttpStatus::OK)
        .addHeader("Content-Type", MimeTypes::getType("html"))
        .addHeader("Content-Length", "12")
        .setBody("Hello World!");
```

### Build

```bash
make
```

### Run

```bash
./webserv [config_file]
```

