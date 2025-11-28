import {themes as prismThemes} from 'prism-react-renderer';
import type {Config} from '@docusaurus/types';
import type * as Preset from '@docusaurus/preset-classic';

// Simple inline SVG as data URI for the logo (works in all environments)
const logoSvg = `data:image/svg+xml,${encodeURIComponent(`<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 64 64" fill="none"><defs><linearGradient id="sg" x1="0%" y1="0%" x2="100%" y2="100%"><stop offset="0%" style="stop-color:%23e85d4c"/><stop offset="100%" style="stop-color:%23c94a3a"/></linearGradient></defs><circle cx="32" cy="32" r="24" fill="url(%23sg)"/><ellipse cx="24" cy="24" rx="6" ry="4" fill="white" opacity="0.4"/></svg>`)}`;

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
        src: logoSvg,
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
