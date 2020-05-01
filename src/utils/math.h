#ifndef UTILS_MATH_H
#define UTILS_MATH_H

#define cos(angle) ((float)std::cos(glm::radians((angle))))
#define sin(angle) ((float)std::sin(glm::radians((angle))))
#define tan(angle) ((float)std::tan(glm::radians((angle))))

/* used to dovetail axial coordinate pairs
 */
int dovetail(int k1, int k2);

#endif /* UTILS_MATH_H */
