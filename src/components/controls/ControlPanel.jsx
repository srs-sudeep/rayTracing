import { useState } from 'react';
import Tabs from '../ui/Tabs';
import LightControls from './LightControls';
import MaterialControls from './MaterialControls';
import CameraControls from './CameraControls';
import ViewControls from './ViewControls';
import './ControlPanel.css';

const TABS = [
  { id: 'light', label: 'Lights', icon: '☀' },
  { id: 'material', label: 'Material', icon: '◆' },
  { id: 'camera', label: 'Camera', icon: '📷' },
  { id: 'view', label: 'View', icon: '⊞' },
];

function ControlPanel({ 
  lights,
  onLightsChange,
  onAddLight,
  onRemoveLight,
  material,
  onMaterialChange,
  camera,
  onCameraChange,
  onCameraReset,
  view,
  onViewChange,
  disabled,
  wasmModule
}) {
  const [activeTab, setActiveTab] = useState('light');

  const renderContent = () => {
    switch (activeTab) {
      case 'light':
        return (
          <LightControls 
            lights={lights}
            onChange={onLightsChange}
            onAddLight={onAddLight}
            onRemoveLight={onRemoveLight}
            disabled={disabled}
            wasmModule={wasmModule}
          />
        );
      case 'material':
        return (
          <MaterialControls 
            material={material} 
            onChange={onMaterialChange} 
            disabled={disabled} 
          />
        );
      case 'camera':
        return (
          <CameraControls 
            camera={camera} 
            onChange={onCameraChange}
            onReset={onCameraReset}
            disabled={disabled} 
          />
        );
      case 'view':
        return (
          <ViewControls 
            view={view} 
            onChange={onViewChange} 
            disabled={disabled} 
          />
        );
      default:
        return null;
    }
  };

  return (
    <div className="control-panel">
      <Tabs tabs={TABS} activeTab={activeTab} onChange={setActiveTab} />
      <div className="control-content">
        {renderContent()}
      </div>
    </div>
  );
}

export default ControlPanel;
