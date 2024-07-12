# Welcome to light-weight 3d model viewer repository
The idea behind this project is to create a very easy to run 3D model viewer that would allow to open obj, fbx and possibly other formats.
Along with that the scope for this project include the PBR material handling and hierarchy support.
## Why
Sometimes you need to open a 3D model to just have a quick look on it, but Windows' built-in model viewer has no support for fbx files, which is an industry standard when dealing with 3D.
As well, you might have Blender installed, but Blender's fbx importing implementation has its issues and it sometimes can interpret fbx files incorrectly.
This application can serve as quick check in both cases, and would allow for more complex use cases like PBR material setup, in the case you might need that.
## What its capable of right now
Nothing really, unless you're a programmer and have your 3D model in float[] vertices = [a,b,c,d,...] format.
For now if you're going to launch the app you will see rotating cube with a grid representing XZ plane. The navigation, similar to Maya's navigation, is almost done, you can pan, zoom with camera rotation almost done.
