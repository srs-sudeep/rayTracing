import type {SidebarsConfig} from '@docusaurus/plugin-content-docs';

const sidebars: SidebarsConfig = {
  tutorialSidebar: [
    'intro',
    'getting-started',
    'architecture',
    {
      type: 'category',
      label: 'C++ Engine',
      items: [
        'cpp-engine/overview',
        'cpp-engine/vec3',
        'cpp-engine/ray',
        'cpp-engine/material',
        'cpp-engine/light',
        'cpp-engine/camera',
        'cpp-engine/sphere',
        'cpp-engine/plane',
        'cpp-engine/scene',
        'cpp-engine/renderer',
      ],
    },
    {
      type: 'category',
      label: 'React Frontend',
      items: [
        'react-frontend/overview',
        'react-frontend/hooks',
        'react-frontend/canvas',
        'react-frontend/controls',
        'react-frontend/ui-components',
      ],
    },
    {
      type: 'category',
      label: 'Features',
      items: [
        'features/ray-tracing',
        'features/reflections',
        'features/shadows',
        'features/materials',
        'features/camera-controls',
        'features/scene-presets',
      ],
    },
    'api-reference',
    'deployment',
  ],
};

export default sidebars;
