#include "Cube.h"

Cube::Cube() {

}
Cube::~Cube() {

}

void Cube::initialize(Vertices& vtx, Indices& ind) {
    float vtx2[8][4] = { {-1, -1, -1, 1},{1, -1, -1, 1},
                       {1, -1, 1, 1}, {-1, -1, 1, 1},
                       {-1, 1, -1, 1}, {1, 1, -1, 1},
                       {1, 1, 1, 1}, {-1, 1, 1, 1}
    };

    float col[8][4] = { {1, 0, 0, 1},{0, 1, 0, 1},
                        {0, 0, 1, 1}, {1, 1, 0, 1},
                        {1, 0, 1, 1}, {0, 1, 1, 1},
                        {0.5, 0.5, 0, 1}, {0.5, 0, 0.5, 1}
    };

    // using a for loop push the vertex information into the m_vertices structure 
    // by invoking the  m_vertices.push_back() function.
    // note that you will have to assemble the data for each vertex (i.e., pos and col)
    for (int i = 0; i < 8; i++) {
        Vertex v = Vertex(Vector4f(vtx2[i][0], vtx2[i][1], vtx2[i][2], vtx2[i][3]),
            Vector4f(col[i][0], col[i][1], col[i][2], col[i][3]));
        vtx.push_back(v);
    }

    int indices[] = {
        // front face
        0, 1, 4,
        1, 5, 4,
        // top face
        4, 5, 6,
        6, 7, 4,
        // back face
        2, 6, 7,
        3, 2, 7,
        // bottom face
        0, 2, 1,
        2, 0, 3,
        // left face
        0, 4, 7,
        0, 7, 3,
        // right face
        1, 2, 5,
        2, 6, 5
    };

    for (int i = 0; i < 36; i++) {
        ind.push_back(indices[i]);
    }
}

void Cube::render(Shader shader, Camera cam, int time) {
    Matrix4f rotMat;  // rotation matrix;
    Matrix4f scaleMat; // scaling matrix;
    Matrix4f transMat;	// translation matrix
    Matrix4f modelMat;	// final model matrix
    Matrix4f modelViewMat;	// view matrix
    Matrix4f modelViewProjMat;	// projection matrix

    shader.useProgram(1);
    shader.copyIntVectorToShader(&time, 1, 1, "time");

    modelMat = Matrix4f::identity();
    scaleMat = Matrix4f::scale(scale.x, scale.y, scale.z);
    modelMat = scaleMat * modelMat;
    rotMat = Matrix4f::rotateRollPitchYaw(rollAngle, pitchAngle, yawAngle, 1);
    modelMat = rotMat * modelMat;
    transMat = Matrix4f::translation(position);
    modelMat = transMat * modelMat;
    //shader.copyMatrixToShader(modelMat, "modelMat");
    modelViewMat = cam.getViewMatrix(NULL) * modelMat;
    modelViewProjMat = cam.getProjectionMatrix(NULL) * modelViewMat;
    shader.copyMatrixToShader(modelViewProjMat, "modelViewProjMat");
    Vector3f pos = cam.getPosition();
    shader.copyFloatVectorToShader((float*)&pos, 1, 3, "camPos");
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}

void Cube::update(float deltaTime) {
    float rotationSpeed = 45.0f;
    rollAngle += rotationSpeed * deltaTime;
    pitchAngle += rotationSpeed * deltaTime;
    yawAngle += rotationSpeed * deltaTime;
}