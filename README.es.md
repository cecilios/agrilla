# Una herramienta para superponer una cuadrícula en cualquier zona de la pantalla, proporcionado una ayuda en la composición y en el dibujo.

AGrilla ayuda a los artistas a crear dibujos precisos y detallados mediante el método de cuadrícula. También es útil en la composición, utilizando la regla de los tercios o la proporción áurea. Ver, por ejemplo, [https://www.pinturayartistas.com/como-dibujar-con-cuadricula-paso-a-paso/](https://www.pinturayartistas.com/como-dibujar-con-cuadricula-paso-a-paso/).



## Licencia
AGrilla se distribuye bajo la licencia MIT, una licencia permisiva de código abierto que permite su uso en cualquier proyecto, ya sea de código abierto o propietario. Esta licencia es compatible con la GPL, por lo que puede utilizar el código de AGrilla en proyectos con licencia GPL.

Para más detalles, consulta el archivo [LICENSE](LICENSE).



## Cómo AGrilla puede ayudarte

En la composición.
* Ajusta las dimensines de la rejilla al tamaño del papel o lienzo que vas a utilizar y desplazala sobre la imagen de referencia para encontrar el encuadre que más te guste.
* Elije la rejilla con las líneas de las proporciones áureas para situar sobre ellas los puntos focales importantes, como el sujeto principal o elementos de interés.


Durante el dibujo:
* Localizar las lineas del centro o los tercipos de la imagen. Elije el número de divisiones de la cuadrícula, 2 ó 3, para localizar rápidamente el centro de la imagen o las líneas que la dividen en tercios y que ayudan en la composición.
* Ayuda para dibujar. Elije un mayor número de divisiones de la cuadrícula, para dibujar con mayor precisión las partes más complicadas del modelo.


## Funcionalidades
* **Superposición de una cuadrícula personalizable:**
AGrilla te permite superponer una cuadrícula personalizable sobre cualquier imagen, proporcionando una guía para ayudarte en el dibujo. Elige el tamaño y las divisiones que desees para adaptarla a las necesidades de tu proyecto.

* **Personalización de cuadrícula:**
Ajusta muy fácilmente y sin complicaciones el color y el grosor de las líneas de la cuadrícula según tus preferencias y necesidades, gracias a su interfaz sencilla e intuitiva que no requiere estudiar ningún manual. Tambien puedes elegir el tipo de cuadrícula: regular o basada en las proporciones aureas.

* **Ajuste de la cuadricula al tamaño del lienzo:**
Simplemente indica el tamaño (ancho y alto) del papel o lienzo que vayas a usar. La cuadrícula adoptará inmediatamente la misma razón de aspecto que tu lienzo. Y cuando amplies o disminuyas la rejilla, arrastrando con el ratón sus bordes, se mantendrá automáticamente la razón de aspecto elegida.


## Instalación

Acabo de empezar a publicar el proyecto y, de momento, sólo puede instalarse compilando los fuentes. Pero es muy sencillo y rápido.

### Paso 1: Requisitos
- CMake >= 3.10
- Un compilador compatible con C/C++
- Las librerías de wxWidgets.

Estos requisitos normalmente ya están instalados en sistemas Linux.

### Paso 2: Descargar el código fuente

Supongamos que tienes una carpeta llamada `proyectos`. Crea una subcarpeta para Agrilla y descárga los funtes desde el repositorio principal en GitHub:

```
cd proyectos
git clone -b main https://github.com/cecilios/agrilla.git
cd agrilla
```

### Paso 3: Genera los makefiles

Una vez descargados los fuentes, crea una carpeta para la compilación (p. ej., `my-build`):

```
mkdir my-build
```

Ahora tendrás la siguiente estructura de carpetas:

```
    proyectos
       ├── agrilla
               ├── src
               ├── include
               ├── my-build
               ├──  ...
```

Ahora ejecuta cmake para generar el makefile y comprobar las dependencias necesarias. Si cmake falla, instala las dependencias que falten y repite este paso hasta que la orden se ejecute correctamente:

```
cd my-build
cmake -G "Unix Makefiles" ../
```

### Paso 4: Compilando el proyecto

Ahora puedes compilar el código fuente y compilar. Es muy rápido:

```
make
```

Si la orden anterior finaliza correctamente, puedes probar el programa:

```
./agrilla
```

O puedes instalar y ejecutar el programa:

```
sudo make install
agrilla
```

Agradezco tus comentarios y correcciones a estas instrucciones. Gracias.



## Contribuir al proyecto

Si deseas contribuir a este proyecto, eres bienvenido/a. El proyecto no tiene fines comerciales y todas las contribuciones son bienvenidas.

Al contribuir al repositorio de AGrilla mediante solicitudes de incorporación de cambios, comentarios u otros medios, el colaborador libera su contenido bajo los términos de licencia y derechos de autor descritos en la [LICENCIA](LICENCIA).
Cada colaborador posee los derechos de autor sobre sus contribuciones a AGrilla.

Lee [Contribuir](CONTRIBUTING.md) para obtener información sobre cómo contribuir.


## Ayuda, informes de errores y comentarios

No dudes en hacer preguntas o enviar comentarios y sugerencias para mejorar. Visita la [página de problemas](https://github.com/cecilios/AGrilla/issues) de AGrilla y crea un nuevo asunto.

¡Muchas gracias por ayudar a mejorar AGrilla!




