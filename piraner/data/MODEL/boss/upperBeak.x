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
 5.69914;-0.03669;2.07608;,
 4.11340;2.70697;2.07608;,
 4.11340;0.92426;2.07408;,
 0.28508;6.68188;2.07608;,
 0.28508;0.84519;2.07408;,
 -3.54324;2.70697;2.07608;,
 -3.54323;0.92427;2.07408;,
 -5.12898;-0.03669;2.07608;,
 -3.54323;0.92427;2.07408;,
 -5.12898;-0.03669;2.07608;,
 0.28508;-1.15260;-17.24324;,
 -3.54324;2.70697;2.07608;,
 0.28508;6.68188;2.07608;,
 4.11340;2.70697;2.07608;,
 5.69914;-0.03669;2.07608;,
 4.11340;0.92426;2.07408;,
 0.28508;0.84519;2.07408;,
 -3.54323;0.92427;2.07408;;
 
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
  0.000959;-0.001122;0.999999;,
  0.001941;-0.001122;0.999997;,
  0.000758;-0.000862;0.999999;,
  -0.000267;-0.000602;1.000000;,
  -0.001553;-0.001122;0.999998;,
  -0.001941;-0.001122;0.999997;,
  -0.001033;-0.000862;0.999999;,
  0.000113;-0.000602;1.000000;,
  0.175417;-0.974947;0.136777;,
  0.254296;-0.954937;0.153065;,
  0.508887;-0.839355;0.191093;,
  -0.835539;0.482914;-0.262046;,
  -0.769704;0.579305;-0.268257;,
  -0.693351;0.667780;-0.270802;,
  0.693351;0.667781;-0.270802;,
  0.769704;0.579305;-0.268257;,
  0.835540;0.482913;-0.262046;,
  -0.508886;-0.839356;0.191093;,
  -0.254297;-0.954937;0.153065;,
  -0.000001;-0.994695;0.102871;;
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
