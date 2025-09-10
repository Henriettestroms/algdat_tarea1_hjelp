import numpy as np
import os

# ✱ ENDRING: robuste stier mot code/sorting/data/array_input
BASE_DIR = os.path.dirname(os.path.abspath(__file__))                         # ✱ ENDRING
OUT_DIR  = os.path.join(BASE_DIR, "..", "data", "array_input")               # ✱ ENDRING
os.makedirs(OUT_DIR, exist_ok=True)                                          # ✱ ENDRING

def generar_arreglo(n, tipo, dominio):
    if dominio == "D1":
        valores = np.arange(10)
    elif dominio == "D7":
        valores = np.arange(10**7 + 1)
    else:
        raise ValueError("Dominio no reconocido")
    
    if tipo == "ascendente":
        return np.sort(np.random.choice(valores, n, replace=True))
    elif tipo == "descendente":
        return np.sort(np.random.choice(valores, n, replace=True))[::-1]
    elif tipo == "aleatorio":
        return np.random.choice(valores, n, replace=True)
    else:
        raise ValueError("Tipo de ordenamiento no reconocido")

def guardar_arreglo(nombre_archivo, arreglo):
    # ✱ ENDRING: skriv n først (matcher C++-stdin-formatet ditt)
    path = os.path.join(OUT_DIR, nombre_archivo)                              # ✱ ENDRING
    with open(path, "w") as f:
        f.write(str(len(arreglo)))                                            # ✱ ENDRING
        f.write(" ")
        f.write(" ".join(map(str, arreglo)))                                  # ✱ ENDRING

def generar_archivos():
    N = [10**1, 10**3, 10**5]  # 10**7 er svært tungt; legg til ved behov
    T = ["ascendente", "descendente", "aleatorio"]
    D = ["D1", "D7"]
    M = ["a", "b", "c"]
    
    for n in N:
        for t in T:
            for d in D:
                for m in M:
                    nombre_archivo = f"{n}_{t}_{d}_{m}.txt"
                    arreglo = generar_arreglo(n, t, d)
                    guardar_arreglo(nombre_archivo, arreglo)
                    print(f"Generado: {nombre_archivo} -> {OUT_DIR}")

if __name__ == "__main__":
    generar_archivos()
