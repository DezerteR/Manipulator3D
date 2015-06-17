#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <chrono>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <type_traits>
#include <memory>
#include <dirent.h>


#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include <vector>
#include <list>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <fstream>
#include <ctime>
#include <cmath>
#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#define GLM_FORCE_SSE2
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/projection.hpp>

#include <glm/gtx/simd_mat4.hpp>
#include <glm/gtx/simd_vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/matrix_major_storage.hpp>
#include <glm/gtx/perpendicular.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtx/optimum_pow.hpp>

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
