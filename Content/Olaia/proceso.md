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

