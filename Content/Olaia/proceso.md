# PASO 1: Implementación de Smooth Locomotion en VR (Unreal VR Template)

## Objetivo

Añadir movimiento continuo ("smooth locomotion") al VR Template de Unreal sin eliminar todavía el sistema de teleport original.

---

##  Cambios realizados

### 1. Creación de un VRPawn propio

Para evitar modificar directamente el VR Template original:

- Se realizó una copia del `VRPawn` del template.
- La copia se llama:

```
OlaiaVRPawn
```

A partir de ahora todas las modificaciones se realizan sobre este blueprint.

#### Motivo

Mantener intacto el VR Template original para:

- Evitar romper funcionalidades existentes.
- Poder comparar comportamientos.
- Tener una copia de seguridad funcional.

---

### 2. Cambio de GameMode

Se modificó el GameMode del proyecto para que utilice el nuevo Pawn.

---


### 3. Reorganización del Input Mapping Context (IMC)

#### Situación original

El VR Template utilizaba:

```
IA_Move
```

para controlar el teleport.

#### Cambio realizado

Se creó una nueva Input Action:

```
IA_SmoothMove
```

---

#### Nuevo esquema de controles

##### Joystick izquierdo

```
IA_SmoothMove
```

- X → Movimiento lateral (strafe)
- Y → Avance / retroceso

##### Joystick derecho

Se mantiene para:

- Snap Turn (eje X)

##### Teleport

Sigue asociado a:

```
IA_Move
```


---

### 5. Modificación del IMC

Dentro del Input Mapping Context:

Se añadió:

```
IA_SmoothMove
```
Con sus bindings correspondientes para el joystick izquierdo.
---


## Blueprint de Smooth Move

Se ha editado el blueprint para añadir el movimiento. La velocidad se puede cambiar con la variable MoveSpeed.
---


# PASO 2: Sistema básico de munición para la pistola

## Objetivo

Añadir un sistema de munición a la pistola del VR Template para controlar cuántas balas quedan disponibles y limitar el disparo cuando se agoten.

---

## Cambios realizados

### 1. Análisis del Blueprint de la pistola

Se decidió reutilizar el Blueprint de la pistola proporcionado por el VR Template. Se ha creado un child blueprint.

La lógica existente de disparo se mantiene intacta y se amplía con nuevas variables relacionadas con la munición.

---

### 2. Creación de variables de munición

Se añadieron dos variables de tipo Integer:

```
CurrentAmmo
```

Número de balas disponibles actualmente en el cargador.

Ejemplo:

```
15
```

---

```
MaxAmmo
```

Cantidad máxima de munición disponible para el arma.

Ejemplo:

```
100
```

---

### 3. Modificación de la lógica de disparo

Se localizó la función/evento responsable del disparo dentro del Blueprint de la pistola.

Antes de ejecutar el disparo se comprueba:

```
CurrentAmmo > 0
```

Si la condición es verdadera:

- Se realiza el disparo.
- Se reduce la munición actual en una unidad.

Si la condición es falsa:

- El arma no dispara.

---

### 4. Consumo de munición

Cada vez que se dispara:

```
CurrentAmmo = CurrentAmmo - 1
```

Esto permite llevar un control de las balas restantes en el cargador.

---

## Estado actual

El sistema permite:

- Definir la munición disponible.
- Reducir la munición al disparar.
- Impedir disparos cuando la munición llega a cero.

# PASO 3: Sistema de vidas, daño e invulnerabilidad temporal

## Objetivo

Implementar un sistema básico de vida para el jugador, junto con una mecánica de daño con invulnerabilidad temporal (3 segundos) para evitar recibir daño continuo.

---

## Cambios realizados

### 1. Sistema de vidas en el VRPawn

Se añadieron variables al `OlaiaVRPawn`:

```
CurrentLives
```

- Tipo: Integer  
- Representa las vidas actuales del jugador

---

### 2. Sistema de invulnerabilidad

Se añadió la variable:

```
bIsInvulnerable
```

- Tipo: Boolean  
- Controla si el jugador puede recibir daño

---

## 3. Lógica de daño

Se creó una función en el VRPawn:

```
TakeDamage
```

### Funcionamiento:

- Si el jugador está en estado de invulnerabilidad → no recibe daño
- Si no lo está:
  - Se resta 1 vida a `CurrentLives`
  - Se activa `bIsInvulnerable`

---

## 4. Invulnerabilidad temporal

Tras recibir daño:

- Se activa un timer de 3 segundos
- Durante ese tiempo el jugador no puede volver a recibir daño
- Pasado el tiempo, la invulnerabilidad se desactiva

---

## 5. Sistema de pruebas (enemigo temporal)

Para testear el sistema sin enemigos implementados:

- Se creó un Blueprint con una **Sphere Collision**
- Al detectar el jugador:
  - Llama a `TakeDamage`

Esto simula el comportamiento de un enemigo de forma básica.

---

## Estado actual

El sistema permite:

- Control de vidas del jugador
- Recepción de daño controlada
- Invulnerabilidad temporal tras recibir daño
- Pruebas de combate mediante una esfera en el nivel


# PASO 4: Game Over al llegar a 0 vidas

## Objetivo

Hacer que el juego termine cuando el jugador llegue a 0 vidas, integrándolo dentro del sistema de daño ya creado.

---

## Cambios realizados

### 1. Modificación del sistema de daño

Dentro de la función:

```
TakeDamage
```

tras restar 1 vida al jugador:

```
CurrentLives = CurrentLives - 1
```

se añade una comprobación de fin de partida.

---

## 2. Comprobación de vidas

Se añade una condición:

```
CurrentLives <= 0
```

---

### Resultado de la condición:

#### ✔ TRUE (0 o menos vidas)

Se ejecuta Game Over.

Opciones actuales:

- Reiniciar el nivel con:
```
Open Level (Current Level)
```

o

- Finalizar el juego con:
```
Quit Game
```

---

#### ✖ FALSE (aún hay vidas)

El juego continúa normalmente sin cambios.

---

## Estado actual

El sistema completo de vida ahora incluye:

- Reducción de vida al recibir daño
- Invulnerabilidad temporal de 3 segundos
- Bloqueo de daño durante invulnerabilidad
- Final de partida al llegar a 0 vidas

---

## Flujo general

```
TakeDamage
   ↓
¿bIsInvulnerable?
   ├── TRUE → No hacer nada
   └── FALSE:
           ↓
      CurrentLives - 1
           ↓
      ¿CurrentLives <= 0?
           ├── TRUE → Game Over
           └── FALSE → Continuar juego
```

---

## Trabajo pendiente

- Añadir pantalla de Game Over en VR
- Añadir efectos visuales/sonoros al morir
- Añadir menú de reinicio en VR