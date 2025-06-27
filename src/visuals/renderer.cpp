#include "renderer.h"
#include "components/graphics-component.h"
#include "data-parser.h"

Renderer::Renderer(sf::Window* window) : window_(window), camera_(std::make_shared<Camera>()), time_(0) {
	textures_ = DataParser::getTextureData();

	// Constructs a square
	float vertices[] = {
		// positions		   texture coords
		 0.5f,  0.5f,  0.0f,    1.0f,  1.0f,   // top right
		 0.5f, -0.5f,  0.0f,	1.0f,  0.0f,   // bottom right
		-0.5f, -0.5f,  0.0f,	0.0f,  0.0f,   // bottom left
		-0.5f,  0.5f,  0.0f,    0.0f,  1.0f    // top left
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	// Set up the TextureData objects
	for (auto& texture: textures_) {
		TextureData* data = &texture.second;
		glGenVertexArrays(1, &data->VAO);
		glGenBuffers(1, &data->VBO);
		glGenBuffers(1, &data->VEO);
		glGenBuffers(1, &data->instanceVBO);

		glBindVertexArray(data->VAO);

		// Instance buffer
		// Translations
		glm::vec3 instanceData[256];
		for (int i = 0; i < 256; i += 2) {
			// Translations
			instanceData[i] = glm::vec3(3.0, 3.0, 0.0);
			// Colour
			instanceData[i + 1] = glm::vec3(1.0, 0.0, 0.0);
		}
		glBindBuffer(GL_ARRAY_BUFFER, data->instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 256, &instanceData[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		data->instanceBufferSize = 256;
		
		glBindBuffer(GL_ARRAY_BUFFER, data->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->VEO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

		// Texture Coords
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		// Offsets
		glBindBuffer(GL_ARRAY_BUFFER, data->instanceVBO);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glVertexAttribDivisor(2, 1);

		// Colours
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribDivisor(3, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenTextures(1, &data->textureId);
		glBindTexture(GL_TEXTURE_2D, data->textureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data->width, data->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data->data);
		// Optional, for low resolution textures when far away
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data->data);
	}

	// enable certain functionality
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	createShaders_();
}

std::shared_ptr<Camera> Renderer::getCamera() {
	return camera_;
}

void Renderer::createShaders_() {
	// Need to make it more sophisticated in the future (ex. reading shaders from files)
	std::ifstream vertexIn("shaders/vertex.glsl");
	assert(vertexIn.good());
	std::string vertexShaderSource((std::istreambuf_iterator<char>(vertexIn)), std::istreambuf_iterator<char>());
	vertexIn.close();

	std::ifstream fragmentIn("shaders/fragment.glsl");
	assert(fragmentIn.good());
	std::string fragmentShaderSource((std::istreambuf_iterator<char>(fragmentIn)), std::istreambuf_iterator<char>());
	fragmentIn.close();


	// I have to do it this way, because OpenGl expects a C type string
	const char* vertexSourceCStr = vertexShaderSource.c_str();
	const char* fragmentSourceCStr = fragmentShaderSource.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSourceCStr, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		// Gonna need better logging method
		std::cout << "ERROR!!! Vertex shader compilation failed!\n" << infoLog << '\n';
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSourceCStr, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR!!! Fragment shader compilation failed!\n" << infoLog << '\n';
	}

	shaderProgram_ = glCreateProgram();
	glAttachShader(shaderProgram_, vertexShader);
	glAttachShader(shaderProgram_, fragmentShader);
	glLinkProgram(shaderProgram_);

	glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram_, 512, NULL, infoLog);
		std::cout << "ERROR!!! Shader program linking failed!\n" << infoLog << '\n';
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

glm::vec3 Renderer::getWorldMousePos() {
	sf::Vector2i pixel = sf::Mouse::getPosition(*window_);
	glm::vec3 win{float(pixel.x), float(window_->getSize().y - pixel.y), 0.0f};

	sf::Vector2f sz = sf::Vector2f(window_->getSize()) * camera_->getZoom();
	glm::mat4 proj = glm::ortho(-sz.x / 2, sz.x / 2, -sz.y / 2, sz.y / 2, 0.1f, 1000.0f);

	glm::mat4 view = camera_->getView();
	glm::vec4 viewport{ 0, 0, float(window_->getSize().x), float(window_->getSize().y)};
	return glm::unProject(win, view, proj, viewport);
}

void Renderer::addComponent(std::shared_ptr<GraphicsComponent> component) {
	componentsToModify_.push(ComponentManagement(component, true, component->textureName_));
}

void Renderer::addComponent(std::shared_ptr<GraphicsComponent> component, const std::string& newTexture) {
	componentsToModify_.push(ComponentManagement(component, true, newTexture));
}

void Renderer::removeComponent(std::shared_ptr<GraphicsComponent> component) {
	componentsToModify_.push(ComponentManagement(component, false));
}

void Renderer::removeComponent_(std::shared_ptr<GraphicsComponent> component) {
	TextureData& data = textures_.at(component->textureName_);
	if (data.activeInstances <= 0) {
		return;
	}


	data.components.at(component->id_ ) = *(data.components.end() - 1);
	auto& newComponent = data.components.at(component->id_);
	newComponent->id_ = component->id_;
	newComponent->makeDirty();

	data.components.pop_back();

	// making inactive vertices not render, instead of deleting them, because deallocation and allocation are both expensive
	data.activeInstances--;
}

void Renderer::addComponent_(std::shared_ptr<GraphicsComponent> component, std::string textureName) {
	// throws an exception if the texture was wrong
	assert(textures_.find(textureName) != textures_.end());
	
	component->textureName_ = textureName;

	TextureData& data = textures_.at(textureName);
	data.components.push_back(component);

	component->setId(data.activeInstances);

	glBindVertexArray(data.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, data.instanceVBO);

	// checks for free space
	data.activeInstances++;

	if (data.activeInstances > data.instanceData.size() / 2) {
		data.instanceData.push_back(glm::vec3(0.0, 0.0, 0.0));
		data.instanceData.push_back(glm::vec3(1.0, 1.0, 1.0));
	}

	if (data.activeInstances > data.instanceBufferSize / 2) {
		// Kinda works like std::vector, but for OpenGl buffer
		data.instanceBufferSize *= 2;
		glBufferData(GL_ARRAY_BUFFER, data.instanceBufferSize * sizeof(float) * 3, nullptr, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glVertexAttribDivisor(2, 1);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribDivisor(3, 1);
		glBufferSubData(GL_ARRAY_BUFFER, 0, data.instanceData.size() * sizeof(glm::vec3), data.instanceData.data());
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	component->makeDirty();
}

void Renderer::draw(float deltaTime) {
	time_ += deltaTime;
	camera_->update(deltaTime);

	// so that it doesn't try to create or delete anything while calculating vertices
	while (!componentsToModify_.empty()) {
		ComponentManagement& componentData = componentsToModify_.front();
		if (componentData.create) {
			addComponent_(componentData.component, componentData.newTexture);
		}
		else {
			removeComponent_(componentData.component);
		}
		componentsToModify_.pop();
	}

	for (auto& texture : textures_) {
		for (auto& component : texture.second.components) {
			if (!component->getDirty()) {
				continue;
			}
			component->calculate_(texture.second);
		}
	}

	glClearColor(0.7f, 0.95f, 0.97f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram_);

	// best way to combine scaling->rotations->translations

	// model matrix transforms local space -> global space

	// view matrix transforms global space -> view space
	// y is up, right handed
	// translating the scene of the reverse direction where I want to move
	// moving the "camera" means moving the entire world while keeping our "camera" stationary lol
	// so this line moves the entire world forward, because I want to move the "camera" backward

	// view space -> clip space
	glm::mat4 projection;
	sf::Vector2f bounds = (sf::Vector2f)window_->getSize() * camera_->getZoom();
	projection = glm::ortho(-bounds.x / 2, bounds.x / 2, -bounds.y / 2, bounds.y / 2, 0.1f, 1000.0f);
	// projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 300.0f);

	// sending the matrices to the shader
	unsigned int matrixLocation = glGetUniformLocation(shaderProgram_, "view");
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(camera_->getView()));

	matrixLocation = glGetUniformLocation(shaderProgram_, "projection");
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(projection));

	// Allows for more than one texture at once (for normal maps I guess)
	glActiveTexture(GL_TEXTURE0);
	for (auto& texture : textures_) {
		// this one is diffrent for every texture, hence I'm sending it here
		// would probably be better to set this as an instanced buffer, but this is easier and I'm most probably going to change it anyway
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, -texture.second.origin);

		// scaling the texture
		model = glm::scale(model, texture.second.dimensions);

		matrixLocation = glGetUniformLocation(shaderProgram_, "model");
		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, texture.second.textureId);
		glBindVertexArray(texture.second.VAO);
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, texture.second.activeInstances);
		glBindBuffer(GL_ARRAY_BUFFER, texture.second.instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	window_->display();
}
