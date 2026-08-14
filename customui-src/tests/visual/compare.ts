// tests/visual/compare.ts
import fs from 'node:fs';
import path from 'node:path';
import { PNG } from 'pngjs';
import pixelmatch from 'pixelmatch';

export interface CompareResult {
  pass: boolean;
  diffRatio: number;
  width: number;
  height: number;
}

export function comparePngs(
  actualBuf: Buffer,
  baselinePath: string,
  diffOutPath: string,
  threshold = 0.03,
): CompareResult {
  if (!fs.existsSync(baselinePath)) {
    fs.mkdirSync(path.dirname(baselinePath), { recursive: true });
    fs.writeFileSync(baselinePath, actualBuf);
    return { pass: true, diffRatio: 0, width: 0, height: 0 };
  }
  const actual = PNG.sync.read(actualBuf);
  const baseline = PNG.sync.read(fs.readFileSync(baselinePath));
  if (actual.width !== baseline.width || actual.height !== baseline.height) {
    return { pass: false, diffRatio: 1, width: actual.width, height: actual.height };
  }
  const diff = new PNG({ width: actual.width, height: actual.height });
  const px = pixelmatch(
    actual.data,
    baseline.data,
    diff.data,
    actual.width,
    actual.height,
    { threshold: 0.05 },
  );
  const ratio = px / (actual.width * actual.height);
  if (ratio > threshold) {
    fs.mkdirSync(path.dirname(diffOutPath), { recursive: true });
    fs.writeFileSync(diffOutPath, PNG.sync.write(diff));
  }
  return { pass: ratio <= threshold, diffRatio: ratio, width: actual.width, height: actual.height };
}
