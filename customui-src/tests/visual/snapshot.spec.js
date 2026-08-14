"use strict";
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
var __generator = (this && this.__generator) || function (thisArg, body) {
    var _ = { label: 0, sent: function() { if (t[0] & 1) throw t[1]; return t[1]; }, trys: [], ops: [] }, f, y, t, g = Object.create((typeof Iterator === "function" ? Iterator : Object).prototype);
    return g.next = verb(0), g["throw"] = verb(1), g["return"] = verb(2), typeof Symbol === "function" && (g[Symbol.iterator] = function() { return this; }), g;
    function verb(n) { return function (v) { return step([n, v]); }; }
    function step(op) {
        if (f) throw new TypeError("Generator is already executing.");
        while (g && (g = 0, op[0] && (_ = 0)), _) try {
            if (f = 1, y && (t = op[0] & 2 ? y["return"] : op[0] ? y["throw"] || ((t = y["return"]) && t.call(y), 0) : y.next) && !(t = t.call(y, op[1])).done) return t;
            if (y = 0, t) op = [op[0] & 2, t.value];
            switch (op[0]) {
                case 0: case 1: t = op; break;
                case 4: _.label++; return { value: op[1], done: false };
                case 5: _.label++; y = op[1]; op = [0]; continue;
                case 7: op = _.ops.pop(); _.trys.pop(); continue;
                default:
                    if (!(t = _.trys, t = t.length > 0 && t[t.length - 1]) && (op[0] === 6 || op[0] === 2)) { _ = 0; continue; }
                    if (op[0] === 3 && (!t || (op[1] > t[0] && op[1] < t[3]))) { _.label = op[1]; break; }
                    if (op[0] === 6 && _.label < t[1]) { _.label = t[1]; t = op; break; }
                    if (t && _.label < t[2]) { _.label = t[2]; _.ops.push(op); break; }
                    if (t[2]) _.ops.pop();
                    _.trys.pop(); continue;
            }
            op = body.call(thisArg, _);
        } catch (e) { op = [6, e]; y = 0; } finally { f = t = 0; }
        if (op[0] & 5) throw op[1]; return { value: op[0] ? op[1] : void 0, done: true };
    }
};
Object.defineProperty(exports, "__esModule", { value: true });
var test_1 = require("@playwright/test");
var node_path_1 = require("node:path");
var node_url_1 = require("node:url");
var compare_1 = require("./compare");
var __filename = (0, node_url_1.fileURLToPath)(import.meta.url);
var __dirname = node_path_1.default.dirname(__filename);
var BASELINES = node_path_1.default.join(__dirname, '__baselines__');
var DIFFS = node_path_1.default.join(__dirname, '__diffs__');
var SCENARIOS = [
    'flat-forward-5s',
    'left-turn',
    'slope-climb',
    'plow-on-field',
    'figure-eight',
];
var _loop_1 = function (id) {
    (0, test_1.test)("snapshot:".concat(id), function (_a) { return __awaiter(void 0, [_a], void 0, function (_b) {
        var buf, baselinePath, diffPath, THRESHOLD, result;
        var page = _b.page;
        return __generator(this, function (_c) {
            switch (_c.label) {
                case 0: return [4 /*yield*/, page.goto("/?standalone=1&scene=".concat(id))];
                case 1:
                    _c.sent();
                    // Deterministic delay: Scenario at t=3000ms. All scenarios have duration >= 3s.
                    return [4 /*yield*/, page.waitForTimeout(3000)];
                case 2:
                    // Deterministic delay: Scenario at t=3000ms. All scenarios have duration >= 3s.
                    _c.sent();
                    return [4 /*yield*/, page.screenshot({
                            fullPage: false,
                            clip: { x: 0, y: 0, width: 1280, height: 720 },
                        })];
                case 3:
                    buf = _c.sent();
                    baselinePath = node_path_1.default.join(BASELINES, "".concat(id, ".png"));
                    diffPath = node_path_1.default.join(DIFFS, "".concat(id, "-diff.png"));
                    THRESHOLD = 0.05;
                    result = (0, compare_1.comparePngs)(buf, baselinePath, diffPath, THRESHOLD);
                    console.log("[visual] ".concat(id, " diff ratio=").concat(result.diffRatio.toFixed(4)));
                    (0, test_1.expect)(result.pass, "Snapshot ".concat(id, " exceeded diff threshold: ratio=").concat(result.diffRatio.toFixed(4), " (> ").concat(THRESHOLD, "). Diff: ").concat(diffPath)).toBeTruthy();
                    return [2 /*return*/];
            }
        });
    }); });
};
for (var _i = 0, SCENARIOS_1 = SCENARIOS; _i < SCENARIOS_1.length; _i++) {
    var id = SCENARIOS_1[_i];
    _loop_1(id);
}
