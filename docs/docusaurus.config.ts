import {themes as prismThemes} from 'prism-react-renderer';
import type {Config} from '@docusaurus/types';
import type * as Preset from '@docusaurus/preset-classic';

const config: Config = {
  title: 'RayTracer Studio',
  tagline: 'Real-time CPU Ray Tracing in the Browser',
  favicon: 'img/favicon.svg',

  url: 'https://ray-tracing.vercel.app',
  baseUrl: '/docs/',

  organizationName: 'srs-sudeep',
  projectName: 'rayTracing',

  onBrokenLinks: 'warn',
  onBrokenMarkdownLinks: 'warn',

  i18n: {
    defaultLocale: 'en',
    locales: ['en'],
  },

  presets: [
    [
      'classic',
      {
        docs: {
          sidebarPath: './sidebars.ts',
          editUrl: 'https://github.com/srs-sudeep/rayTracing/tree/main/docs/',
          routeBasePath: '/',
        },
        blog: false,
        theme: {
          customCss: './src/css/custom.css',
        },
      } satisfies Preset.Options,
    ],
  ],

  themeConfig: {
    image: 'img/raytracer-social.png',
    navbar: {
      title: 'RayTracer Studio',
      logo: {
        alt: 'RayTracer Studio Logo',
        src: 'data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZpZXdCb3g9IjAgMCA2NCA2NCIgZmlsbD0ibm9uZSI+PGRlZnM+PGxpbmVhckdyYWRpZW50IGlkPSJzZyIgeDE9IjAlIiB5MT0iMCUiIHgyPSIxMDAlIiB5Mj0iMTAwJSI+PHN0b3Agb2Zmc2V0PSIwJSIgc3R5bGU9InN0b3AtY29sb3I6I2U4NWQ0YyIvPjxzdG9wIG9mZnNldD0iMTAwJSIgc3R5bGU9InN0b3AtY29sb3I6I2M5NGEzYSIvPjwvbGluZWFyR3JhZGllbnQ+PC9kZWZzPjxjaXJjbGUgY3g9IjMyIiBjeT0iMzIiIHI9IjI0IiBmaWxsPSJ1cmwoI3NnKSIvPjxlbGxpcHNlIGN4PSIyNCIgY3k9IjI0IiByeD0iNiIgcnk9IjQiIGZpbGw9IndoaXRlIiBvcGFjaXR5PSIwLjQiLz48L3N2Zz4=',
        href: '/',
        width: 32,
        height: 32,
      },
      items: [
        {
          type: 'docSidebar',
          sidebarId: 'tutorialSidebar',
          position: 'left',
          label: 'Documentation',
        },
        {
          href: 'https://ray-tracing.vercel.app/',
          label: '🎨 Launch App',
          position: 'right',
        },
        {
          href: 'https://github.com/srs-sudeep/rayTracing',
          label: 'GitHub',
          position: 'right',
        },
      ],
    },
    footer: {
      style: 'dark',
      links: [
        {
          title: 'Docs',
          items: [
            {
              label: 'Getting Started',
              to: '/intro',
            },
            {
              label: 'C++ Engine',
              to: '/cpp-engine/overview',
            },
            {
              label: 'React Frontend',
              to: '/react-frontend/overview',
            },
          ],
        },
        {
          title: 'More',
          items: [
            {
              label: 'GitHub',
              href: 'https://github.com/srs-sudeep/rayTracing',
            },
            {
              label: 'Live Demo',
              href: 'https://ray-tracing.vercel.app/',
            },
          ],
        },
      ],
      copyright: `Copyright © ${new Date().getFullYear()} RayTracer Studio. Built with Docusaurus.`,
    },
    prism: {
      theme: prismThemes.github,
      darkTheme: prismThemes.dracula,
      additionalLanguages: ['cpp', 'bash'],
    },
    colorMode: {
      defaultMode: 'dark',
      disableSwitch: false,
      respectPrefersColorScheme: true,
    },
  } satisfies Preset.ThemeConfig,
};

export default config;
