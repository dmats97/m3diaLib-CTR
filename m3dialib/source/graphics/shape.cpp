#include "graphics/drawables/shape.hpp"

namespace m3d {
    Shape::Shape() :
        m_changed(true),
        m_interpolationMode(m3d::GPU_INTERPOLATE) { /* do nothing */ }

    Shape::~Shape() {
        linearFree(m_internalVertices);
    }

    void Shape::addVertex(m3d::Vertex t_vertex) {
        m_vertices.push_back(t_vertex);
        m_changed = true;
    }

    void Shape::addVertex(int t_x, int t_y, int t_z, m3d::Color t_color) {
        m3d::Vertex vertex = { { (float) t_x, (float) t_y, (float) t_z }, t_color };
        m_vertices.push_back(vertex);
        m_changed = true;
    }

    void Shape::clearVertices() {
        m_vertices.clear();
        linearFree(m_internalVertices);
        m_changed = true;
    }

    void Shape::setInterpolationMode(m3d::InterpolationMode t_interpolationMode) {
        m_interpolationMode = t_interpolationMode;
    }

    m3d::InterpolationMode Shape::getInterpolationMode() {
        return m_interpolationMode;
    }

    void Shape::draw(int, int, int, int) {
        if (m_changed) {
            m_changed = false;
            linearFree(m_internalVertices);
            linearFree(m_elementData);
            m_internalVertices = static_cast<m3d::InternalVertex*>(linearAlloc(m_vertices.size() * sizeof(m3d::InternalVertex)));

            if (m_internalVertices == nullptr) return;


            for (unsigned int i = 0; i < m_vertices.size(); i++) {
                float x = m_vertices[i].position.x,
                      y = m_vertices[i].position.y,
                    red = (float) m_vertices[i].color.getRed() / 255,
                  green = (float) m_vertices[i].color.getGreen() / 255,
                   blue = (float) m_vertices[i].color.getBlue() / 255,
                  alpha = (float) m_vertices[i].color.getAlpha() / 255;

                m_internalVertices[i] = (m3d::InternalVertex) { {x, y, 0.5f}, {red, green, blue, alpha} };
            }

            // TODO: Improve this (a lot)
            for (unsigned int i = 0; i < m_vertices.size(); i++) {
                if (i % 3 == 0 && i != 0) m_indices.push_back(static_cast<s16>(i - 1));

                m_indices.push_back(static_cast<s16>(i));
            }

            m_elementData = static_cast<s16*>(linearAlloc(m_indices.size() * sizeof(s16)));

            for (unsigned int i = 0; i < m_indices.size(); i++) {
                m_elementData[i] = m_indices[i];
            }
        }

        // there be dragons
        C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
        AttrInfo_Init(attrInfo);
        AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // position
        AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 4); // color

        C3D_BufInfo* bufInfo = C3D_GetBufInfo();
        BufInfo_Init(bufInfo);
        BufInfo_Add(bufInfo, m_internalVertices, sizeof(m3d::InternalVertex), 2, 0x10);

        C3D_TexEnv* env = C3D_GetTexEnv(0);
        C3D_TexEnvSrc(env, C3D_Both, GPU_PRIMARY_COLOR, 0, 0);
        C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
        C3D_TexEnvFunc(env, C3D_Both, m_interpolationMode);

        // Draw the VBO
        C3D_DrawElements(GPU_TRIANGLE_FAN, m_indices.size(), GPU_UNSIGNED_BYTE, m_elementData);
    }
} /* m3d */
