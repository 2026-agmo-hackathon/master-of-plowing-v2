#!/usr/bin/env node
// GLTFLoader references `self` (browser global) when handling textures.
// Polyfill before importing so texture decode paths find URL/Blob.
if (typeof globalThis.self === 'undefined') {
  globalThis.self = globalThis;
}

import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader.js';
import { readFileSync } from 'node:fs';
import path from 'node:path';
import * as THREE from 'three';

const arg = process.argv[2];
if (!arg) {
  console.error('Usage: node inspect-gltf.mjs <path>');
  process.exit(1);
}
const fullPath = path.resolve(arg);
const buf = readFileSync(fullPath);

// Convert Node Buffer to ArrayBuffer slice (GLTFLoader.parse expects ArrayBuffer)
const arrayBuffer = buf.buffer.slice(buf.byteOffset, buf.byteOffset + buf.byteLength);

const loader = new GLTFLoader();
loader.parse(
  arrayBuffer,
  '',
  (gltf) => {
    console.log(`# GLTF: ${fullPath}`);
    console.log(`# Scenes: ${gltf.scenes.length}, Animations: ${gltf.animations.length}`);

    console.log('## Bones');
    let boneCount = 0;
    gltf.scene.traverse((node) => {
      if (node.isBone) {
        const pos = node.getWorldPosition(new THREE.Vector3());
        console.log(
          `- ${node.name}  parent=${node.parent?.name ?? '(root)'}  pos=(${pos.x.toFixed(2)},${pos.y.toFixed(2)},${pos.z.toFixed(2)})`
        );
        boneCount++;
      }
    });
    if (boneCount === 0) {
      console.log('(no Bone nodes found)');
    }

    console.log('## Meshes');
    let meshCount = 0;
    gltf.scene.traverse((node) => {
      if (node.isMesh) {
        const mat = Array.isArray(node.material)
          ? node.material.map((m) => m.name || m.type).join(',')
          : node.material?.name || node.material?.type || '(none)';
        console.log(`- ${node.name} (geo=${node.geometry.type}) material=${mat}`);
        meshCount++;
      }
    });
    if (meshCount === 0) {
      console.log('(no Mesh nodes found)');
    }

    console.log('## Animations');
    if (gltf.animations.length === 0) {
      console.log('(no animation clips)');
    }
    for (const clip of gltf.animations) {
      console.log(`- ${clip.name} (duration=${clip.duration}s)`);
    }

    console.log('## Hierarchy');
    const print = (node, depth = 0) => {
      console.log('  '.repeat(depth) + '- ' + node.name + ' [' + node.type + ']');
      for (const child of node.children) print(child, depth + 1);
    };
    print(gltf.scene);
  },
  (err) => {
    console.error(err);
    process.exit(1);
  }
);
