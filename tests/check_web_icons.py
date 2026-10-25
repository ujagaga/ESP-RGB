"""Check embedded SVG routes, cache versions, and page references without hardware."""
import hashlib
import re
import unittest
import xml.etree.ElementTree as ET
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class WebIconsTest(unittest.TestCase):
    def test_assets_and_references(self):
        header = (ROOT / 'ui.h').read_text()
        server = (ROOT / 'http_server.cpp').read_text()
        assets = dict(re.findall(r'ICON_(\w+)\[\] PROGMEM = R"svg\((.*?)\)svg";', header, re.S))
        routes = re.findall(r'\{"(/icons/[^" ]+)", ICON_(\w+)\}', header)
        self.assertEqual(len(assets), 3)
        self.assertEqual(len(routes), len(assets))
        for url, key in routes:
            svg = assets[key]
            self.assertEqual(ET.fromstring(svg).tag, '{http://www.w3.org/2000/svg}svg')
            digest = hashlib.sha256(svg.encode()).hexdigest()[:12]
            self.assertTrue(url.endswith('-' + digest + '.svg'), url)
            self.assertNotIn('currentColor', svg)  # External images cannot inherit page colors.
        referenced = set(re.findall(r'/icons/[\w-]+\.svg', header.split('struct IconAsset')[0]))
        self.assertEqual(referenced, {url for url, _ in routes})
        self.assertNotIn('<svg', server)


if __name__ == '__main__':
    unittest.main()
