#include "GeometricObjects/Mesh.hpp"

Mesh::Mesh(const string &fileName): Object() {
    nom = fileName;
    load(fileName);
}

Mesh::~Mesh() {
    if (cares.size() > 0) cares.clear();
    if (vertexs.size() > 0) vertexs.clear();
}

void Mesh::makeTriangles() {
}


/*
bool Mesh::hit (Ray& r, float tmin, float tmax, ShadeInfo &shadeInfo) const {   

    bool hit_any = false;
    float closest_t = tmax;

    for (const Face& face : cares) {  // Recorremos todas las caras (triángulos)
        // Obtener los vértices del triángulo
        vec3 A = vec3(vertexs[face.idxVertices[0]]);
        vec3 B = vec3(vertexs[face.idxVertices[1]]);
        vec3 C = vec3(vertexs[face.idxVertices[2]]);

        // Calcular la normal del triángulo
        vec3 normal = normalize(cross(B - A, C - A));

        // Resolver la ecuación del plano: (P - A) · normal = 0
        float denom = dot(normal, r.direction);
        if (abs(denom) < 1e-6) continue; // El rayo es paralelo al plano, no hay intersección

        float t = dot(A - r.origin, normal) / denom;
        if (t < tmin || t > closest_t) continue; // Fuera del rango válido

        // Calcular el punto de intersección con el plano
        vec3 P = r.origin + t * r.direction;

        // Comprobamos si el punto P está dentro del triángulo usando coordenadas baricéntricas
        vec3 v0 = B - A, v1 = C - A, v2 = P - A;
        float d00 = dot(v0, v0);
        float d01 = dot(v0, v1);
        float d11 = dot(v1, v1);
        float d20 = dot(v2, v0);
        float d21 = dot(v2, v1);

        float denom_inv = 1.0f / (d00 * d11 - d01 * d01);
        float u = (d11 * d20 - d01 * d21) * denom_inv;
        float v = (d00 * d21 - d01 * d20) * denom_inv;
        float w = 1.0f - u - v;

        // Verificamos si u, v y w están dentro del rango [0, 1]
        if (u >= 0 && v >= 0 && w >= 0) {
            hit_any = true;
            closest_t = t;

            // Guardamos información sobre la intersección
            shadeInfo.t = t;
            shadeInfo.p = P;
            shadeInfo.normal = normal;
            shadeInfo.mat = this->material;
        }
    }

    return hit_any;


}
*/

/*

bool Mesh::hit(Ray& r, float tmin, float tmax, ShadeInfo &shadeInfo) const {   
    bool hit_any = false;
    float closest_t = tmax;

    for (const Face& face : cares) {  // Recorremos todas las caras (triángulos)
        // Obtener los vértices del triángulo
        vec3 A = vec3(vertexs[face.idxVertices[0]]);
        vec3 B = vec3(vertexs[face.idxVertices[1]]);
        vec3 C = vec3(vertexs[face.idxVertices[2]]);

        // Calcular la normal del triángulo
        vec3 normal = normalize(cross(B - A, C - A));

        // Resolver la ecuación del plano: (P - A) · normal = 0
        float denom = dot(normal, r.direction);
        if (abs(denom) < 1e-6) continue; // El rayo es paralelo al plano, no hay intersección

        float t = dot(A - r.origin, normal) / denom;
        if (t < tmin || t > closest_t) continue; // Fuera del rango válido

        // Calcular el punto de intersección con el plano
        vec3 P = r.origin + t * r.direction;

        // Cálculo del área del triángulo ABC
        vec3 AB = B - A;
        vec3 AC = C - A;
        float areaABC = 0.5f * length(cross(AB, AC));

        // Cálculo de las áreas de los triángulos ABP, BCP, y CAP
        vec3 AP = P - A;
        vec3 BP = P - B;
        vec3 BC = C - B;
        float areaABP = 0.5f * length(cross(AB, AP)); // Área de ABP
        float areaBCP = 0.5f * length(cross(BC, BP)); // Área de BCP
        float areaCAP = 0.5f * length(cross(AC, AP)); // Área de CAP

        float u = areaABP / areaABC; 
        float v = areaBCP / areaABC; 
        float w = areaCAP / areaABC; 

        // Verificar si u, v y w están dentro del rango [0, 1] y si su suma es 1
        if (u >= 0 && v >= 0 && w >= 0 && std::abs(u + v + w - 1.0f) < 1e-6) {
            hit_any = true;
            closest_t = t;

            // Guardamos información sobre la intersección
            shadeInfo.t = t;
            shadeInfo.p = P;
            shadeInfo.normal = normal;
            shadeInfo.mat = this->material;
        }
    }

    return hit_any;
}

*/


bool Mesh::hit(Ray& r, float tmin, float tmax, ShadeInfo &shadeInfo) const {
    bool hit_any = false;
    float closest_t = tmax;

    for (const Face& face : cares) {
        vec3 A = vec3(vertexs[face.idxVertices[0]]);
        vec3 B = vec3(vertexs[face.idxVertices[1]]);
        vec3 C = vec3(vertexs[face.idxVertices[2]]);

        vec3 normal = normalize(cross(B - A, C - A));
        float denom = dot(normal, r.direction);
        if (abs(denom) < 1e-6) continue;

        float t = dot(A - r.origin, normal) / denom;
        if (t < tmin || t > closest_t) continue;

        vec3 P = r.origin + t * r.direction;

        // Comprobar si el punto P está dentro del triángulo usando productos cruzados
        vec3 edge1 = B - A;
        vec3 edge2 = C - B;
        vec3 edge3 = A - C;
        
        vec3 C1 = cross(edge1, P - A);
        vec3 C2 = cross(edge2, P - B);
        vec3 C3 = cross(edge3, P - C);

        if (dot(C1, normal) >= 0 && dot(C2, normal) >= 0 && dot(C3, normal) >= 0) {
            hit_any = true;
            closest_t = t;
            shadeInfo.t = t;
            shadeInfo.p = P;
            shadeInfo.normal = normal;
            shadeInfo.mat = this->material;
        }
    }

    return hit_any;
}




bool Mesh::allHits(Ray& r, float tmin, float tmax, vector<ShadeInfo>& listShadeInfos) const {    
    return false;
}

void Mesh::aplicaTG(shared_ptr<TG> t) {    
}

void Mesh::update(int frame) {
}

// Funció per eliminar espais al principi i final d'una cadena
void trim(std::string &str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    str = (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

// Funció per dividir una cadena en parts utilitzant espais com a separador
std::vector<std::string> split(const std::string &str) {
    std::vector<std::string> tokens;
    std::istringstream stream(str);
    std::string token;
    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// Funció per llegir el fitxer i processar el contingut
void Mesh::load(const std::string &fileName) {
    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "Boundary object file not found.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        trim(line);
        if (line.empty()) continue;

        std::vector<std::string> lineParts = split(line);
        if (lineParts.empty()) continue;

        // Si és un comentari
        if (lineParts[0] == "#") {
            // std::cout << "Comment: " << line.substr(1) << std::endl;
        }

        // Si és un vèrtex (v)
        else if (lineParts[0] == "v" && lineParts.size() >= 4) {
            vertexs.push_back(5.5f*glm::vec4(
                std::stof(lineParts[1]), 
                std::stof(lineParts[2]), 
                std::stof(lineParts[3]), 
                1.0f
            ));
        }

        // Si és una normal (vn)
        else if (lineParts[0] == "vn") {
            // Placeholder per afegir normals si és necessari
        }

        // Si és una coordenada de textura (vt)
        else if (lineParts[0] == "vt") {
            // Placeholder per afegir textures si és necessari
        }

        // Si és una cara (f)
        else if (lineParts[0] == "f" && lineParts.size() >= 4) {
            Face face;
            for (size_t i = 1; i <= 3; ++i) {  // Assumeix triangles
                size_t pos = lineParts[i].find('/');
                int idx = (pos != std::string::npos) ? std::stoi(lineParts[i].substr(0, pos)) - 1 : std::stoi(lineParts[i]) - 1;
                face.idxVertices.push_back(idx);
            }
            cares.push_back(face);
        }
    }

    //centerModel();

    file.close();
}


vec3 Mesh::getMinCoords() const {
    // Retorna el vértice con coordenadas mínimas
    glm::vec3 minCoords = vertexs.empty() ? glm::vec3(0.0f) : glm::vec3(vertexs[0]);
    for (const auto& v : vertexs) {
        minCoords = glm::min(minCoords, glm::vec3(v));
    }
    return minCoords;
}

vec3 Mesh::getMaxCoords() const {
    // Retorna el vértice con coordenadas máximas
    glm::vec3 maxCoords = vertexs.empty() ? glm::vec3(0.0f) : glm::vec3(vertexs[0]);
    for (const auto& v : vertexs) {
        maxCoords = glm::max(maxCoords, glm::vec3(v));
    }
    return maxCoords;
}

void Mesh::centerModel() {
    if (vertexs.empty()) return;

    // Calcular bounding box
    glm::vec3 minCoords = getMinCoords();
    glm::vec3 maxCoords = getMaxCoords();

    // Calcular centro del modelo
    glm::vec3 center = (minCoords + maxCoords) * 0.5f;

    // Mover todos los vértices al centro
    for (auto& v : vertexs) {
        v -= glm::vec4(center, 0.0f);
    }

    for (auto& v : vertexs) {
        v *= 0.01f;
    }
    
    
}


// mover a un punto concreto
void Mesh::translate(const vec3 &punto) {
    if (vertexs.empty()) return;

    // Calcular el centro del objeto
    vec3 center(0.0f);
    for (const auto &vertex : vertexs) {
        center.x += vertex.x;
        center.y += vertex.y;
        center.z += vertex.z;
    }
    center /= static_cast<float>(vertexs.size());

    // Calcular el vector de traslación desde el centro actual a la posición objetivo
    vec3 translation = punto - center;

    // Aplicar la traslación a todos los vértices
    for (auto &vertex : vertexs) {
        vertex.x += translation.x;
        vertex.y += translation.y;
        vertex.z += translation.z;
    }
}

