const ACCESSIBLE = {
  WHEN_UNLOCKED: 0,
  AFTER_FIRST_UNLOCK: 1,
  ALWAYS: 2,
  WHEN_PASSCODE_SET_THIS_DEVICE_ONLY: 3,
  WHEN_UNLOCKED_THIS_DEVICE_ONLY: 4,
  AFTER_FIRST_UNLOCK_THIS_DEVICE_ONLY: 5,
  ALWAYS_THIS_DEVICE_ONLY: 6,
};

const SecureStorage = {
  setItem: jest.fn().mockResolvedValue(true),
  getItem: jest.fn().mockResolvedValue('value'),
  setItems: jest.fn().mockResolvedValue(null),
  getAllItems: jest.fn().mockResolvedValue([{ key: 'key', value: 'value' }]),
  removeItem: jest.fn().mockResolvedValue(true),
  getAllKeys: jest.fn().mockResolvedValue(['key']),
};

const mock = {
  default: SecureStorage,
  ACCESSIBLE,
};

module.exports = mock;
