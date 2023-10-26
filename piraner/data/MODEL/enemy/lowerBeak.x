xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 18;
 1.81717;0.62063;3.38580;,
 1.28493;0.29998;3.42057;,
 1.28493;-0.29468;3.48697;,
 0.00000;0.32636;3.41765;,
 0.00000;-1.62073;3.63355;,
 -1.28493;0.29998;3.42057;,
 -1.28493;-0.29468;3.48697;,
 -1.81717;0.62063;3.38580;,
 1.28493;0.29998;3.42057;,
 1.81717;0.62063;3.38580;,
 0.00000;0.25898;-3.13581;,
 1.28493;-0.29468;3.48697;,
 0.00000;-1.62073;3.63355;,
 -1.28493;-0.29468;3.48697;,
 -1.81717;0.62063;3.38580;,
 -1.28493;0.29998;3.42057;,
 0.00000;0.32636;3.41765;,
 1.28493;0.29998;3.42057;;
 
 14;
 3;0,1,2;,
 3;1,3,2;,
 3;2,3,4;,
 3;3,5,4;,
 3;4,5,6;,
 3;5,7,6;,
 3;8,9,10;,
 3;9,11,10;,
 3;11,12,10;,
 3;12,13,10;,
 3;13,14,10;,
 3;14,15,10;,
 3;15,16,10;,
 3;16,17,10;;
 
 MeshMaterialList {
  5;
  14;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.672000;0.440000;0.192000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.104000;0.080000;0.040000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.040000;0.024000;0.008000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  20;
  -0.000956;0.110971;0.993823;,
  -0.001931;0.110971;0.993822;,
  -0.000755;0.110717;0.993852;,
  0.000264;0.110462;0.993880;,
  0.001541;0.110971;0.993823;,
  0.001931;0.110971;0.993822;,
  0.001026;0.110717;0.993851;,
  -0.000113;0.110462;0.993880;,
  -0.175531;0.984144;0.025470;,
  -0.254465;0.966088;0.043829;,
  -0.509200;0.855451;0.094444;,
  0.836151;-0.508843;-0.204766;,
  0.770334;-0.605439;-0.200072;,
  0.693975;-0.693749;-0.192640;,
  -0.693975;-0.693749;-0.192640;,
  -0.770334;-0.605439;-0.200072;,
  -0.836151;-0.508843;-0.204766;,
  0.509200;0.855451;0.094444;,
  0.254465;0.966088;0.043829;,
  0.000000;0.999947;-0.010281;;
  14;
  3;1,0,2;,
  3;0,7,2;,
  3;2,7,3;,
  3;7,6,3;,
  3;3,6,4;,
  3;6,5,4;,
  3;9,10,8;,
  3;11,12,12;,
  3;12,13,12;,
  3;14,15,15;,
  3;15,16,15;,
  3;17,18,17;,
  3;18,19,8;,
  3;19,9,8;;
 }
 MeshTextureCoords {
  18;
  0.500000;1.000000;,
  0.676777;0.926777;,
  0.323223;0.926777;,
  0.750000;0.750000;,
  0.250000;0.750000;,
  0.676777;0.573223;,
  0.323223;0.573223;,
  0.500000;0.500000;,
  0.250000;0.500000;,
  0.312500;0.500000;,
  0.500000;0.000000;,
  0.375000;0.500000;,
  0.437500;0.500000;,
  0.500000;0.500000;,
  0.562500;0.500000;,
  0.625000;0.500000;,
  0.687500;0.500000;,
  0.750000;0.500000;;
 }
}
