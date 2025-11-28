import clsx from 'clsx';
import Link from '@docusaurus/Link';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';
import Layout from '@theme/Layout';
import Heading from '@theme/Heading';
import styles from './index.module.css';

function HomepageHeader() {
  const {siteConfig} = useDocusaurusContext();
  
  // Detect if we're in local development
  const isLocalDev = typeof window !== 'undefined' && 
    (window.location.hostname === 'localhost' || window.location.hostname === '127.0.0.1');
  
  const appUrl = isLocalDev ? 'http://localhost:5173' : 'https://ray-tracing.vercel.app/';
  
  return (
    <header className={clsx('hero hero--primary', styles.heroBanner)}>
      <div className="container">
        <div className={styles.heroContent}>
          <div className={styles.heroText}>
            <Heading as="h1" className={styles.heroTitle}>
              RayTracer <span className={styles.highlight}>Studio</span>
            </Heading>
            <p className={styles.heroSubtitle}>{siteConfig.tagline}</p>
            <div className={styles.buttons}>
              <Link
                className="button button--secondary button--lg"
                to="/intro">
                Get Started →
              </Link>
              <Link
                className="button button--outline button--lg"
                href={appUrl}>
                Launch App ↗
              </Link>
            </div>
          </div>
          <div className={styles.heroImage}>
            <div className={styles.sphereDemo}>
              <div className={styles.sphere}></div>
              <div className={styles.shadow}></div>
            </div>
          </div>
        </div>
      </div>
    </header>
  );
}

const features = [
  {
    title: 'C++ Ray Tracing Engine',
    icon: '⚡',
    description: 'High-performance ray tracing written in C++ and compiled to WebAssembly for near-native browser performance.',
  },
  {
    title: 'Interactive Controls',
    icon: '🎛️',
    description: 'Real-time manipulation of lights, materials, camera, and scene with instant visual feedback.',
  },
  {
    title: 'Realistic Reflections',
    icon: '🪞',
    description: 'Recursive ray tracing with Fresnel effect for physically accurate mirror surfaces.',
  },
  {
    title: 'Blinn-Phong Shading',
    icon: '💡',
    description: 'Professional lighting model with ambient, diffuse, and specular components.',
  },
  {
    title: 'Shadow Rays',
    icon: '🌑',
    description: 'Hard shadows from point lights add depth and realism to the scene.',
  },
  {
    title: '3D Ground Plane',
    icon: '📐',
    description: 'Blender-style infinite grid with axis highlighting and distance fade.',
  },
];

function Feature({icon, title, description}) {
  return (
    <div className={styles.feature}>
      <div className={styles.featureIcon}>{icon}</div>
      <Heading as="h3">{title}</Heading>
      <p>{description}</p>
    </div>
  );
}

function HomepageFeatures() {
  return (
    <section className={styles.features}>
      <div className="container">
        <Heading as="h2" className={styles.featuresTitle}>Features</Heading>
        <div className={styles.featureGrid}>
          {features.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
      </div>
    </section>
  );
}

function TechStack() {
  return (
    <section className={styles.techStack}>
      <div className="container">
        <Heading as="h2" className={styles.featuresTitle}>Technology Stack</Heading>
        <div className={styles.techGrid}>
          <div className={styles.techCard}>
            <span className={styles.techIcon}>🔧</span>
            <strong>C++</strong>
            <span>Core Engine</span>
          </div>
          <div className={styles.techCard}>
            <span className={styles.techIcon}>🌐</span>
            <strong>WebAssembly</strong>
            <span>Runtime</span>
          </div>
          <div className={styles.techCard}>
            <span className={styles.techIcon}>⚛️</span>
            <strong>React</strong>
            <span>UI Framework</span>
          </div>
          <div className={styles.techCard}>
            <span className={styles.techIcon}>⚡</span>
            <strong>Vite</strong>
            <span>Build Tool</span>
          </div>
          <div className={styles.techCard}>
            <span className={styles.techIcon}>🔗</span>
            <strong>Emscripten</strong>
            <span>Compiler</span>
          </div>
        </div>
      </div>
    </section>
  );
}

export default function Home(): JSX.Element {
  const {siteConfig} = useDocusaurusContext();
  return (
    <Layout
      title="Documentation"
      description="Real-time CPU Ray Tracing in the Browser - Documentation">
      <HomepageHeader />
      <main>
        <HomepageFeatures />
        <TechStack />
      </main>
    </Layout>
  );
}
