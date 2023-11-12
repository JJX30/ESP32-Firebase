import logo from "./logo.svg";
import { useState, useEffect } from "react";
import { db } from "./firebase";
import { getDatabase, ref, child, set, get, onValue } from "firebase/database";
import "./App.css";

function App() {
  const [LDR, setLDR] = useState(0);
  const [button, setButton] = useState(false);
  const [digital, setDigital] = useState(false);
  const [analog, setAnalog] = useState(0);

  useEffect(() => {
    const numberRef = ref(db, "Sensor/ldr_data");
    const unsubscribe = onValue(numberRef, (snapshot) => {
      const data = snapshot.val();
      setLDR(data);
    });

    // Clean up listener
    return () => unsubscribe();
  });

  useEffect(() => {
    const numberRef = ref(db, "Sensor/switch");
    const unsubscribe = onValue(numberRef, (snapshot) => {
      const data = snapshot.val();
      setButton(data);
    });

    // Clean up listener
    return () => unsubscribe();
  });

  const handleChange = (e) => {
    setAnalog(e.target.value);
    set(ref(db, "LED/analog"), Number(analog));
  };

  const handleClick = () => {
    if (digital) {
      setDigital(false);
      set(ref(db, "LED/digital"), false);
    } else {
      setDigital(true);
      set(ref(db, "LED/digital"), true);
    }
  };

  return (
    <div className="App">
      <h1 className="display-1">ESP32 Web Server</h1>
      <div className="body">
        <h4 className="form-label">LDR Data: {LDR}</h4>
        <input
          type="range"
          className="range form-range"
          id="customRange1"
          min={0}
          max={1500}
          readOnly
          value={LDR}
        ></input>

        <br />
        <br />
        <h4 className="form-label">LED status: {digital ? "ON" : "OFF"}</h4>
        <button type="button" onClick={handleClick} className="btn btn-primary">
          Click me to switch digital
        </button>
        <br />
        <br />
        <h4 className="form-label">
          Physical button status: {button ? "PRESSED" : "NOT PRESSED"}
        </h4>
        <br />
        <br />
        <h4 className="form-label">Analog slider: {analog}</h4>
        <input
          type="range"
          className="range form-range"
          id="customRange1"
          min={0}
          max={255}
          onChange={handleChange}
          value={analog}
        ></input>
      </div>
    </div>
  );
}

export default App;
