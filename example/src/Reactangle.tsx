import { useEffect, useRef } from "react";
import {
  Animated,
  Dimensions,
  Easing,
  StyleSheet,
  Text,
  View,
} from "react-native";

export const Rectangle = () => {
  const animationValue = useRef(new Animated.Value(0)).current;
  const colorAnimation = useRef(new Animated.Value(0)).current;

  // Get the full width of the screen
  const screenWidth = Dimensions.get("window").width;

  useEffect(() => {
    const animate = () => {
      Animated.loop(
        Animated.parallel([
          Animated.sequence([
            Animated.timing(animationValue, {
              toValue: -200,
              duration: 600,
              easing: Easing.inOut(Easing.quad),
              useNativeDriver: false,
            }),
            Animated.timing(animationValue, {
              toValue: 0,
              duration: 600,
              easing: Easing.inOut(Easing.quad),
              useNativeDriver: false,
            }),
          ]),
          Animated.sequence([
            Animated.timing(colorAnimation, {
              toValue: 1,
              duration: 600,
              easing: Easing.inOut(Easing.quad),
              useNativeDriver: false,
            }),
            Animated.timing(colorAnimation, {
              toValue: 0,
              duration: 600,
              easing: Easing.inOut(Easing.quad),
              useNativeDriver: false,
            }),
          ]),
        ]),
      ).start();
    };

    animate();
  }, [animationValue, colorAnimation, screenWidth]);

  const backgroundColor = colorAnimation.interpolate({
    inputRange: [0, 1],
    outputRange: ["blue", "green"],
  });

  return (
    <View style={styles.container}>
      <Animated.View
        style={[
          styles.rectangle,
          { transform: [{ translateX: animationValue }], backgroundColor },
        ]}
      >
        <Text style={styles.text}>It stops when JS Thread blocked</Text>
      </Animated.View>
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    justifyContent: "center",
    alignItems: "center",
    marginVertical: 20,
    width: "100%",
  },
  rectangle: {
    width: 150,
    height: 150,
    backgroundColor: "blue",
    borderRadius: 10,
    marginLeft: 200,
    justifyContent: "center",
    alignItems: "center",
  },
  text: {
    color: "white",
    fontSize: 16,
    fontWeight: "bold",
    textAlign: "center",
  },
});
