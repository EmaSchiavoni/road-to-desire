
**Tabla de contenidos**

[TOCM]

# Road To Desire

Road To Desire es un juego de plataformas 2D desarrollado como proyecto final para una materia universitaria. El proyecto original fue desarrollado completamente por mi y recibió una calificación de **10/10**.


## 📄 Descripción

El juego tiene una temática inspirada en *Dragon Ball*, donde el jugador controla al personaje principal y debe superar obstáculos en cada nivel, recolectando monedas y buscando las **7 esferas del dragón**. El juego fue diseñado para contar con **16 niveles**, pero en la versión presentada, debido a los tiempos de entrega, se incluyeron **3 niveles funcionales** (aunque los 16 aparecen en el mapa).

Además del juego, se desarrolló una **webapp para la construcción visual de niveles**, permitiendo exportarlos en formato JSON, el cual es utilizado por el juego para cargar los niveles.


## 💥 Desafíos técnicos

El desarrollo del juego se realizó **sin un motor de videojuegos**, utilizando únicamente la biblioteca **SFML**, que solo proporciona [algunas clases](https://www.sfml-dev.org/documentation/2.5.1/annotated.php) (como Texture, Sprite, Sound, Drawable) a modo de interfaces para no tener que realizar implementaciones de, relativamente, bajo nivel y que ademas sean funcionales independientemente del sistema operativo. Esto, además de aumentar la dificultad por no tener las ayudas visuales de un motor gráfico, significó que varias funcionalidades esenciales tuvieron que ser implementadas manualmente, incluyendo:

- Detección de colisiones
- Implementación de botones y eventos
- Manejo de animaciones
- Gestión de recursos
- Programación del bucle principal
- Manejo de la variación de fotogramas en el tiempo


## 🛠️ Instrucciones para compilar y ejecutar

### Dependencias

El programa requiere **dos dependencias**:

1. **jsoncpp** (probado en versión 1.9.5)
   - Descargar manualmente la biblioteca.
   - Incluir el código fuente completo (**la carpeta completa**) en el directorio `include` del proyecto.

2. **SFML** (probado en versión 2.5.1)
   - Descargar la biblioteca desde el sitio oficial.
   - Extraer los archivos de la siguiente manera:
     - **DLLs (`bin/`)** → Copiar cada archivo al **directorio raíz** del proyecto.
     - **Headers (`include/`)** → Copiar al **directorio `include/`** del proyecto.
     - **Librerías (`lib/`)** → Copiar al **directorio `lib/`** del proyecto.

> En los directorios `include/` y `lib/` del proyecto se encuentra un **README** con instrucciones sobre el contenido esperado en cada uno.


### Compilación

Para compilar el proyecto, asegúrate de incluir los directorios correspondientes en el compilador y enlazar correctamente las bibliotecas de SFML y jsoncpp. Un ejemplo de compilación con `g++` en sistemas basados en Linux podría ser:

```sh
 g++ -std=c++17 -Iinclude -Llib -o RoadToDesire main.cpp -lsfml-graphics -lsfml-window -lsfml-system -ljsoncpp
```

En Windows, puedes compilar con MinGW o utilizar un entorno como Visual Studio configurando adecuadamente los directorios de inclusión y enlace.


## 📊 Estado del proyecto

Actualmente, **el juego está funcional con 3 niveles completos**, aunque el mapa muestra los 16 niveles planificados. Se encuentra en una versión estable y con posibilidad de expansión futura. Como se comenta anteriormente, los niveles se almacenan en formato json, por lo que es posible leerlos y analizar su estructura para realizar modificaciones o construir nuevos.


## 📜 Licencia

Este proyecto, incluidos todos sus archivos (exceptuando las bibliotecas), está licenciado bajo la GNU General Public License v3.0. Puedes usar, copiar, modificar y distribuir este software libremente, siempre que cualquier trabajo derivado también sea open source y esté bajo la GPLv3.
Consulta el archivo LICENSE que se encuentra en la raìz del proyecto para obtener los términos completos.