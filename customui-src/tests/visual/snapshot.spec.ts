import { test, expect } from '@playwright/test';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { comparePngs } from './compare';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const BASELINES = path.join(__dirname, '__baselines__');
const DIFFS = path.join(__dirname, '__diffs__');

const SCENARIOS = [
  'flat-forward-5s',
  'left-turn',
  'slope-climb',
  'plow-on-field',
  'figure-eight',
] as const;

for (const id of SCENARIOS) {
  test(`snapshot:${id}`, async ({ page }) => {
    await page.goto(`/?standalone=1&scene=${id}`);
    // Deterministic delay: Scenario at t=3000ms. All scenarios have duration >= 3s.
    await page.waitForTimeout(3000);
    const buf = await page.screenshot({
      fullPage: false,
      clip: { x: 0, y: 0, width: 1280, height: 720 },
    });
    const baselinePath = path.join(BASELINES, `${id}.png`);
    const diffPath = path.join(DIFFS, `${id}-diff.png`);
    const THRESHOLD = 0.05;
    const result = comparePngs(buf, baselinePath, diffPath, THRESHOLD);
    console.log(`[visual] ${id} diff ratio=${result.diffRatio.toFixed(4)}`);
    expect(
      result.pass,
      `Snapshot ${id} exceeded diff threshold: ratio=${result.diffRatio.toFixed(4)} (> ${THRESHOLD}). Diff: ${diffPath}`,
    ).toBeTruthy();
  });
}
