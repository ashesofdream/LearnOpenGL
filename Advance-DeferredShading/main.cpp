#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include "../Shader.h"
#include "../variable.h"
#include "../util.h"
#include "../Model.h"
#include "../Camera.h"

using glm::vec3;

int main() {
  int SCRWIDTH = 800, SCRHEIGHT = 600;
  auto window = util::prepare_window();
  auto e_matrix = glm::mat4(1.f);

  Shader g_shader("shaders/g.vs.glsl", "shaders/g.fs.glsl");
  Shader direct_shader("../util/shaders/quad.vs.glsl",
                       "shaders/deferred.fs.glsl");

  GLuint g_buffer;
  glGenFramebuffers(1, &g_buffer);
  glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);

  GLuint g_postion, g_normal, g_color_spec, g_depth;

  glGenTextures(1, &g_postion);
  glBindTexture(GL_TEXTURE_2D, g_postion);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCRWIDTH, SCRHEIGHT, 0, GL_RGB,
               GL_FLOAT, nullptr);
  util::set_texture_prop(GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         g_postion, 0);

  glGenTextures(1, &g_normal);
  glBindTexture(GL_TEXTURE_2D, g_normal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCRWIDTH, SCRHEIGHT, 0, GL_RGB,
               GL_FLOAT, nullptr);
  util::set_texture_prop(GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
                         g_normal, 0);

  glGenTextures(1, &g_color_spec);
  glBindTexture(GL_TEXTURE_2D, g_color_spec);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, SCRWIDTH, SCRHEIGHT, 0, GL_RGBA,
               GL_FLOAT, nullptr);
  util::set_texture_prop(GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
                         g_color_spec, 0);

  glGenRenderbuffers(1, &g_depth);
  glBindRenderbuffer(GL_RENDERBUFFER, g_depth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH, SCRWIDTH, SCRHEIGHT);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, g_depth);

  GLuint attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                           GL_COLOR_ATTACHMENT2};
  glDrawBuffers(3, attachments);
  
  Camera camera(window);
  g_shader.use();
  g_shader.set_mat4("projection", camera.perspective_matrix());
  // model
  Model m("../resource/nanosuit.obj");
  std::vector<glm::vec3> objectPositions;
  objectPositions.emplace_back(-3.0, -3.0, -3.0);
  objectPositions.emplace_back(0.0, -3.0, -3.0);
  objectPositions.emplace_back(3.0, -3.0, -3.0);
  objectPositions.emplace_back(-3.0, -3.0, 0.0);
  objectPositions.emplace_back(0.0, -3.0, 0.0);
  objectPositions.emplace_back(3.0, -3.0, 0.0);
  objectPositions.emplace_back(-3.0, -3.0, 3.0);
  objectPositions.emplace_back(0.0, -3.0, 3.0);
  objectPositions.emplace_back(3.0, -3.0, 3.0);
  // - Colors
  const GLuint NR_LIGHTS = 32;
  std::vector<glm::vec3> lightPositions;
  std::vector<glm::vec3> lightColors;
  srand(13);
  for (GLuint i = 0; i < NR_LIGHTS; i++) {
    // Calculate slightly random offsets
    GLfloat xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
    GLfloat yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
    GLfloat zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
    lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
    // Also calculate random color
    GLfloat rColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
    GLfloat gColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
    GLfloat bColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
    lightColors.push_back(glm::vec3(rColor, gColor, bColor));
  };
  //set light for object
  for (GLuint i = 0; i < lightPositions.size(); i++)
  {
    glUniform3fv(glGetUniformLocation(direct_shader.program_id, ("lights[" + std::to_string(i) + "].Position").c_str()), 1, &lightPositions[i][0]);
    glUniform3fv(glGetUniformLocation(direct_shader.program_id, ("lights[" + std::to_string(i) + "].Color").c_str()), 1, &lightColors[i][0]);
    // Update attenuation parameters and calculate radius
    const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
    const GLfloat linear = 0.7;
    const GLfloat quadratic = 1.8;
    glUniform1f(glGetUniformLocation(direct_shader.program_id, ("lights[" + std::to_string(i) + "].Linear").c_str()), linear);
    glUniform1f(glGetUniformLocation(direct_shader.program_id, ("lights[" + std::to_string(i) + "].Quadratic").c_str()), quadratic);
  }
  //set shader
  direct_shader.use();
  direct_shader.set_vec3("view_pos", camera.pos());
  direct_shader.set_int("color_samp", 0);
  direct_shader.set_int("normal_samp", 1);
  direct_shader.set_int("frag_pos_samp", 2);
  

  glClearColor(0.f,0.f,0.f,0.f);
  auto draw_scene = [&](Shader& s) -> void {
    s.use();
    glBindFramebuffer(GL_FRAMEBUFFER,g_buffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto view_matrix = camera.view_matrix();
    for(int i = 0 ; i < objectPositions.size(); ++i){
        auto model_matrix =  glm::translate(e_matrix, objectPositions[i]); 
        model_matrix = glm::translate(model_matrix, vec3(0.25));
        s.set_mat4("model", model_matrix);
        s.set_mat4("view", view_matrix);
        m.Draw(s);
    }
  };
  glEnable(GL_DEPTH_TEST);
  while (!glfwWindowShouldClose(window)) {
    camera.update();
    //draw_scene(g_shader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    direct_shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,g_color_spec);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,g_normal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,g_postion);
    util::draw_quad();

    glfwSwapBuffers(window);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
}