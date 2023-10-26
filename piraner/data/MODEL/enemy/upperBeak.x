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
 1.81717;-0.52261;2.44552;,
 1.28493;0.39827;2.44552;,
 1.28493;-0.20008;2.44485;,
 0.00000;1.73240;2.44552;,
 0.00000;-0.22662;2.44485;,
 -1.28493;0.39827;2.44552;,
 -1.28493;-0.20007;2.44485;,
 -1.81717;-0.52261;2.44552;,
 -1.28493;-0.20007;2.44485;,
 -1.81717;-0.52261;2.44552;,
 0.00000;-0.89715;-4.03879;,
 -1.28493;0.39827;2.44552;,
 0.00000;1.73240;2.44552;,
 1.28493;0.39827;2.44552;,
 1.81717;-0.52261;2.44552;,
 1.28493;-0.20008;2.44485;,
 0.00000;-0.22662;2.44485;,
 -1.28493;-0.20007;2.44485;;
 
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
  0.000957;-0.001120;0.999999;,
  0.001938;-0.001120;0.999997;,
  0.000757;-0.000861;0.999999;,
  -0.000267;-0.000601;1.000000;,
  -0.001550;-0.001120;0.999998;,
  -0.001938;-0.001120;0.999998;,
  -0.001031;-0.000861;0.999999;,
  0.000113;-0.000601;1.000000;,
  0.175417;-0.974947;0.136777;,
  0.254296;-0.954937;0.153065;,
  0.508892;-0.839352;0.191094;,
  -0.835539;0.482915;-0.262046;,
  -0.769703;0.579306;-0.268257;,
  -0.693350;0.667781;-0.270802;,
  0.693350;0.667781;-0.270802;,
  0.769703;0.579306;-0.268257;,
  0.835539;0.482915;-0.262046;,
  -0.508881;-0.839360;0.191092;,
  -0.254293;-0.954938;0.153063;,
  -0.000004;-0.994695;0.102870;;
  14;
  3;5,4,6;,
  3;4,3,6;,
  3;6,3,7;,
  3;3,2,7;,
  3;7,2,0;,
  3;2,1,0;,
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
  0.500000;0.500000;,
  0.323223;0.573223;,
  0.676777;0.573223;,
  0.250000;0.750000;,
  0.750000;0.750000;,
  0.323223;0.926777;,
  0.676777;0.926777;,
  0.500000;1.000000;,
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
