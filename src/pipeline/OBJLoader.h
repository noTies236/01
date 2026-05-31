#pragma once
// =============================================================================
// OBJLoader.h  -  Minimal Wavefront .obj loader
// -----------------------------------------------------------------------------
// Include AFTER your OpenGL header (glad/glew) and AFTER your Vertex definition.
// Requires GLuint (from GL) and Vertex with 8 floats in this order:
//     pos (x, y, z)   color (r, g, b)   texcoord (u, v)
//
// Normals from the .obj are stored in the color slot, remapped [-1,1] -> [0,1],
// so the model renders shaded by face orientation with no shader changes needed.
//
// Usage:
//     std::vector<Vertex> data;
//     std::vector<GLuint> ibo;
//     if (!loadOBJ("./assets/SmallKey.obj", data, ibo))
//         return -1; // wrong path - relative to executable directory
// =============================================================================

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <array>
#include <algorithm>

inline bool loadOBJ(const std::string& path,
    std::vector<Vertex>& outVertices,
    std::vector<GLuint>& outIndices,
    bool normalize = true)
{
    std::ifstream file(path);
    if (!file.is_open())
        return false;

    std::vector<std::array<float, 3>> positions;
    std::vector<std::array<float, 2>> texcoords;
    std::vector<std::array<float, 3>> normals;
    std::vector<std::vector<std::string>> faces;

    // ---- Phase 1: read the whole file ----
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();  // handle CRLF line endings

        std::istringstream ls(line);
        std::string tag;
        ls >> tag;

        if (tag == "v") {
            std::array<float, 3> p{};
            ls >> p[0] >> p[1] >> p[2];
            positions.push_back(p);
        }
        else if (tag == "vt") {
            std::array<float, 2> t{};
            ls >> t[0] >> t[1];
            texcoords.push_back(t);
        }
        else if (tag == "vn") {
            std::array<float, 3> n{};
            ls >> n[0] >> n[1] >> n[2];
            normals.push_back(n);
        }
        else if (tag == "f") {
            std::vector<std::string> face;
            std::string tok;
            while (ls >> tok) face.push_back(tok);
            faces.push_back(face);
        }
        // all other tags (#, o, g, s, mtllib, usemtl ...) are ignored
    }

    // ---- Normalize: center and scale model to fit clip space [-1, 1] ----
    float cx = 0, cy = 0, cz = 0, scale = 1.0f;
    if (normalize && !positions.empty()) {
        float mnx = 1e30f, mny = 1e30f, mnz = 1e30f;
        float mxx = -1e30f, mxy = -1e30f, mxz = -1e30f;
        for (auto& p : positions) {
            mnx = std::min(mnx, p[0]); mxx = std::max(mxx, p[0]);
            mny = std::min(mny, p[1]); mxy = std::max(mxy, p[1]);
            mnz = std::min(mnz, p[2]); mxz = std::max(mxz, p[2]);
        }
        cx = (mnx + mxx) * 0.5f;
        cy = (mny + mxy) * 0.5f;
        cz = (mnz + mxz) * 0.5f;
        float ext = std::max(mxx - mnx, std::max(mxy - mny, mxz - mnz));
        if (ext > 0.0f) scale = 1.6f / ext;
    }

    // ---- Phase 2: build unique vertices + index buffer ----
    std::unordered_map<std::string, GLuint> uniqueMap;

    for (auto& face : faces) {
        std::vector<GLuint> idx;
        idx.reserve(face.size());

        for (auto& ft : face) {
            auto found = uniqueMap.find(ft);
            if (found != uniqueMap.end()) {
                idx.push_back(found->second);
                continue;
            }

            // parse "v", "v/vt", "v//vn" or "v/vt/vn"  (1-based indices)
            int vi = 0, ti = 0, ni = 0;
            {
                std::string a = ft;
                for (char& c : a) if (c == '/') c = ' ';
                std::istringstream ts(a);
                if (ft.find("//") != std::string::npos) {
                    ts >> vi >> ni;           // v//vn
                }
                else {
                    ts >> vi;
                    if (ts >> ti) ts >> ni;   // v[/vt[/vn]]
                }
            }

            float px = 0, py = 0, pz = 0;
            float r = 0.8f, g = 0.8f, b = 0.8f;  // default gray if no normal
            float u = 0, v = 0;

            if (vi > 0 && vi <= (int)positions.size()) {
                px = (positions[vi - 1][0] - cx) * scale;
                py = (positions[vi - 1][1] - cy) * scale;
                pz = (positions[vi - 1][2] - cz) * scale;
            }
            if (ti > 0 && ti <= (int)texcoords.size()) {
                u = texcoords[ti - 1][0];
                v = texcoords[ti - 1][1];
            }
            if (ni > 0 && ni <= (int)normals.size()) {
                r = normals[ni - 1][0] * 0.5f + 0.5f;  // normal -> color slot
                g = normals[ni - 1][1] * 0.5f + 0.5f;
                b = normals[ni - 1][2] * 0.5f + 0.5f;
            }

            GLuint newIndex = (GLuint)outVertices.size();
            outVertices.push_back(Vertex{ px, py, pz, r, g, b, u, v });
            uniqueMap[ft] = newIndex;
            idx.push_back(newIndex);
        }

        // fan-triangulate the face: (0,1,2), (0,2,3), ...
        for (size_t i = 1; i + 1 < idx.size(); ++i) {
            outIndices.push_back(idx[0]);
            outIndices.push_back(idx[i]);
            outIndices.push_back(idx[i + 1]);
        }
    }

    return true;
}