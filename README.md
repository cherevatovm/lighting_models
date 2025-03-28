# Lighting Models
An app for demonstrating local lighting models.
The app allows you to select a 3D model from a list to which the lighting model will be applied. The following lighting models are available:
1. **Lambert**
2. **Toon Shading**
3. **Oren-Nayar**
   - Includes a roughness parameter (ranging from 0 to 1).
4. **Phong**
   - Includes a specular coefficient as a parameter (must be 1 or greater).
5. **Cook-Torrance**
   - Includes multiple customizable parameters:
     - Roughness (ranging from 0 to 1).
     - Refractive index (must be 1 or greater).
     - Metallic property coefficient (ranging from 0 to 1).
6. **Ashikhmin-Shirley**
   - Includes multiple customizable parameters:
     - Refractive index (must be 1 or greater).
     - **n_u** and **n_v** coefficients that determine the shape of anisotropic specular reflection (must be 0 or greater).
