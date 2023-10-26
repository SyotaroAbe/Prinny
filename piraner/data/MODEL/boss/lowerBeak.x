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
 5.54918;0.71160;2.62990;,
 3.96343;-0.24376;2.73349;,
 3.96344;-2.01546;2.93133;,
 0.13512;-5.96631;3.36804;,
 0.13512;-0.16515;2.72480;,
 -3.69320;-2.01546;2.93133;,
 -3.69320;-0.24376;2.73349;,
 -5.27893;0.71160;2.62990;,
 3.96343;-0.24376;2.73349;,
 5.54918;0.71160;2.62990;,
 0.13512;-0.36590;-16.80056;,
 3.96344;-2.01546;2.93133;,
 0.13512;-5.96631;3.36804;,
 -3.69320;-2.01546;2.93133;,
 -5.27893;0.71160;2.62990;,
 -3.69320;-0.24376;2.73349;,
 0.13512;-0.16515;2.72480;,
 3.96343;-0.24376;2.73349;;
 
 14;
 3;0,1,2;,
 3;2,1,3;,
 3;1,4,3;,
 3;3,4,5;,
 3;4,6,5;,
 3;6,7,5;,
 3;8,9,10;,
 3;9,11,10;,
 3;11,12,10;,
 3;12,13,10;,
 3;13,14,10;,
 3;14,15,10;,
 3;15,16,10;,
 3;16,17,10;;
 
 MeshMaterialList {
  4;
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
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  20;
  -0.001030;0.110720;0.993851;,
  -0.001938;0.110977;0.993821;,
  -0.001549;0.110977;0.993822;,
  -0.000266;0.110463;0.993880;,
  0.000757;0.110720;0.993851;,
  0.001938;0.110977;0.993821;,
  0.000958;0.110977;0.993823;,
  0.000113;0.110463;0.993880;,
  -0.175533;0.984144;0.025471;,
  -0.254468;0.966088;0.043830;,
  -0.509207;0.855446;0.094446;,
  0.836152;-0.508842;-0.204766;,
  0.770335;-0.605438;-0.200072;,
  0.693977;-0.693748;-0.192640;,
  -0.693977;-0.693747;-0.192640;,
  -0.770336;-0.605437;-0.200072;,
  -0.836152;-0.508842;-0.204766;,
  0.509210;0.855444;0.094447;,
  0.254469;0.966087;0.043831;,
  0.000000;0.999947;-0.010281;;
  14;
  3;1,0,2;,
  3;2,0,3;,
  3;0,7,3;,
  3;3,7,4;,
  3;7,6,4;,
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
  0.250000;0.750000;,
  0.750000;0.750000;,
  0.323223;0.573223;,
  0.676777;0.573223;,
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
