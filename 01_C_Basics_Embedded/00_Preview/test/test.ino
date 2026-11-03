void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Hellow from Lince");
  Serial.print("Valor macro __cplusplus: ");
  Serial.println(__cplusplus);

  #if __cplusplus >= 202302L
    Serial.println("Estándar activo: C++23");
  #elif __cplusplus >= 202002L
    Serial.println("Estándar activo: C++20");
  #elif __cplusplus >= 201703L
    Serial.println("Estándar activo: C++17");
  #elif __cplusplus >= 201402L
    Serial.println("Estándar activo: C++14");
  #elif __cplusplus >= 201103L
    Serial.println("Estándar activo: C++11");
  #else
    Serial.println("Estándar activo: C++98 / C++03");
  #endif
}

void loop() {

  #if __cplusplus >= 202302L
    Serial.println("Estándar activo: C++23");
  #elif __cplusplus >= 202002L
    Serial.println("Estándar activo: C++20");
  #elif __cplusplus >= 201703L
    Serial.println("Estándar activo: C++17");
  #elif __cplusplus >= 201402L
    Serial.println("Estándar activo: C++14");
  #elif __cplusplus >= 201103L
    Serial.println("Estándar activo: C++11");
  #else
    Serial.println("Estándar activo: C++98 / C++03");
  #endif

}