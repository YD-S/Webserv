#!/usr/bin/env python3
import cgi

# Encabezado para indicar que el contenido es HTML
print("Content-type:text/html\r\n\r\n")

# Obtener datos del formulario
form = cgi.FieldStorage()

# Función para calcular el resultado
def calcular(num1, num2, operacion):
    if operacion == "add":
        return num1 + num2
    elif operacion == "subtract":
        return num1 - num2
    elif operacion == "multiply":
        return num1 * num2
    elif operacion == "division":
        if num2 != 0:
            return num1 / num2
        else:
            return "Error: División por cero"

# Verificar si se enviaron los parámetros
if "num1" in form and "num2" in form and "operacion" in form:
    try:
        num1 = float(form.getvalue("num1"))
        num2 = float(form.getvalue("num2"))
        operacion = form.getvalue("operacion")
        print("num1:", num1)  # Imprimir num1 para depurar
        print("num2:", num2)  # Imprimir num2 para depurar
        print("operacion:", operacion)  # Imprimir operacion para depurar
        resultado = calcular(num1, num2, operacion)
        print("<h2>El resultado es: {}</h2>".format(resultado))
    except ValueError:
        print("<h2>Por favor, introduce números válidos.</h2>")
else:
    print("<h2>Por favor, introduce todos los valores requeridos.</h2>")
