---
description: Cursor Rules Configuration Guide
type: manual
---

# Cursor Rules Setup for Hackathon Project

This repository contains a comprehensive set of Cursor rules designed to optimize development for hackathon projects. The rules are organized hierarchically and focus on speed, simplicity, and performance.

## Rules Structure

### Core Rules (Always Applied)
- **hackathon-core.md** - Core development principles and standards
- **security.md** - Security best practices and guidelines

### Context-Specific Rules (Auto-Attached)
- **frontend.md** - React/JavaScript frontend development
- **backend.md** - API and server-side development  
- **n8n-integration.md** - n8n workflow automation
- **performance.md** - Performance optimization guidelines
- **database.md** - Database and data management
- **testing.md** - Testing strategies and patterns

## Rule Types Explained

### Always Rules
Applied to every conversation and code generation:
- Core hackathon principles
- Security requirements

### Auto-Attached Rules  
Automatically included when working with matching file patterns:
- Frontend rules for `.js`, `.jsx`, `.ts`, `.tsx` files
- Backend rules for server-side files
- Database rules for database-related files
- Testing rules for test files

### Agent-Requested Rules
Available for the AI to include based on context when needed

## Key Features

### 🚀 Speed Optimized
- Focus on working solutions over perfect code
- Minimal complexity approaches
- Performance-first patterns

### 🔧 Hackathon Specific
- Live data implementations (no mocks)
- Quick iteration patterns
- MVP-focused development

### 📝 Comprehensive Coverage
- Frontend (React, JavaScript, CSS)
- Backend (APIs, databases, authentication)
- n8n workflow automation
- Performance optimization
- Security best practices
- Testing strategies

### 🎨 User Preferences
- Colored console logging with file names
- Code reuse over duplication
- Simple, straightforward approaches

## Usage Guidelines

1. **Development Priority**: Speed over perfection
2. **Code Quality**: Maintain readability and simplicity
3. **Performance**: Always consider optimization
4. **Security**: Never compromise on security basics
5. **Testing**: Test critical paths quickly
6. **Logging**: Use colored output with file identification

## File Organization

```
.cursor/
├── rules/
│   ├── README.md              # This file
│   ├── hackathon-core.md      # Core principles (always)
│   ├── security.md            # Security rules (always)
│   ├── frontend.md            # Frontend development
│   ├── backend.md             # Backend development
│   ├── n8n-integration.md     # n8n workflows
│   ├── performance.md         # Performance optimization
│   ├── database.md            # Database management
│   └── testing.md             # Testing guidelines
```

## Environment Setup

Make sure to configure these environment variables:
- `NODE_ENV` - Environment (development/production)
- `DATABASE_URL` - Database connection string
- `JWT_SECRET` - JWT signing secret
- `N8N_URL` - n8n instance URL
- `N8N_API_KEY` - n8n API key
- `ENCRYPTION_KEY` - Data encryption key

## Logging Standards

All files should include colored logging:
```javascript
console.log('\x1b[36m[filename.js]\x1b[0m', 'Info message');     // Cyan
console.log('\x1b[32m[filename.js]\x1b[0m', 'Success message');  // Green
console.warn('\x1b[33m[filename.js]\x1b[0m', 'Warning message'); // Yellow
console.error('\x1b[31m[filename.js]\x1b[0m', 'Error message');  // Red
```

## Performance Priorities

1. **Database**: Proper indexing and query optimization
2. **Frontend**: Component memoization and lazy loading
3. **Backend**: Connection pooling and caching
4. **Network**: Request deduplication and compression
5. **n8n**: Efficient workflow design and execution

## Security Checklist

- ✅ Input validation and sanitization
- ✅ Authentication and authorization
- ✅ Environment variable configuration
- ✅ Rate limiting implementation
- ✅ Security headers setup
- ✅ Error handling without information disclosure

## Quick Start Commands

```bash
# Set up the project structure
mkdir -p src/{frontend,backend,components,utils,tests}

# Initialize package.json if needed
npm init -y

# Install common dependencies
npm install express cors helmet bcryptjs jsonwebtoken

# Start development
npm run dev
```

## Contributing

When adding new rules:
1. Follow the existing file structure
2. Use appropriate glob patterns for auto-attachment
3. Include colored logging examples
4. Focus on hackathon speed and efficiency
5. Test rules with real code examples
