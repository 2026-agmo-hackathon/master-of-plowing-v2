"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.comparePngs = comparePngs;
// tests/visual/compare.ts
var node_fs_1 = require("node:fs");
var node_path_1 = require("node:path");
var pngjs_1 = require("pngjs");
var pixelmatch_1 = require("pixelmatch");
function comparePngs(actualBuf, baselinePath, diffOutPath, threshold) {
    if (threshold === void 0) { threshold = 0.03; }
    if (!node_fs_1.default.existsSync(baselinePath)) {
        node_fs_1.default.mkdirSync(node_path_1.default.dirname(baselinePath), { recursive: true });
        node_fs_1.default.writeFileSync(baselinePath, actualBuf);
        return { pass: true, diffRatio: 0, width: 0, height: 0 };
    }
    var actual = pngjs_1.PNG.sync.read(actualBuf);
    var baseline = pngjs_1.PNG.sync.read(node_fs_1.default.readFileSync(baselinePath));
    if (actual.width !== baseline.width || actual.height !== baseline.height) {
        return { pass: false, diffRatio: 1, width: actual.width, height: actual.height };
    }
    var diff = new pngjs_1.PNG({ width: actual.width, height: actual.height });
    var px = (0, pixelmatch_1.default)(actual.data, baseline.data, diff.data, actual.width, actual.height, { threshold: 0.05 });
    var ratio = px / (actual.width * actual.height);
    if (ratio > threshold) {
        node_fs_1.default.mkdirSync(node_path_1.default.dirname(diffOutPath), { recursive: true });
        node_fs_1.default.writeFileSync(diffOutPath, pngjs_1.PNG.sync.write(diff));
    }
    return { pass: ratio <= threshold, diffRatio: ratio, width: actual.width, height: actual.height };
}
